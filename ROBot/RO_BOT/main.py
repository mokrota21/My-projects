import discord
import sqlite3
import pandas as pd
import datetime
import requests
import json
from apscheduler.schedulers.asyncio import AsyncIOScheduler

db = sqlite3.connect("discord.db")
with open('schema.sql', mode='r') as f:
    db.cursor().executescript(f.read())
db.commit()

cur = db.cursor()

COUNT = 0


async def load_item_history(limit=3000):
    global COUNT

    for offset in range(limit - 1000, -1, -1000):
        link = 'https://api.prontera.ru/api/v3/ru_prime/item_history/?offset=' + str(offset) + '&limit=1000'

        r = requests.get(link)
        info = json.loads(r.content)

        COUNT = info['count']

        if len(info['results']) == 0:
            continue

        df = pd.DataFrame.from_dict(info['results'], orient='columns')
        df = df[['id', 'player', 'item', 'amount', 'price', 'last_seen']]
        df['seller_id'] = df['player'].apply(lambda x: x['id'])
        df['seller_name'] = df['player'].apply(lambda x: x['name'])
        df['item_id'] = df['item'].apply(lambda x: x['id'])
        df = df.drop('player', axis=1)
        df = df.drop('item', axis=1)

        df.to_sql('purchases', con=db, index=False, if_exists='append')


async def update():
    r = requests.get('https://api.prontera.ru/api/v3/ru_prime/store_data/?offset=0&limit=1&item__id=0')
    info = json.loads(r.content)
    limit = info['count'] + 100
    link = 'https://api.prontera.ru/api/v3/ru_prime/store_data/?offset=0&limit=' + str(limit) + '&item__id=0'

    r = requests.get(link)
    info = json.loads(r.content)

    df = pd.DataFrame.from_dict(info['results'], orient='columns')
    df = df[['id', 'name', 'item', 'store', 'amount', 'price']]
    df['item_name'] = df['name']
    df['x'] = df['store'].apply(lambda x: x['last_x'])
    df['y'] = df['store'].apply(lambda x: x['last_y'])
    df['item_id'] = df['item'].apply(lambda x: x['id'])
    df['store_id'] = df['store'].apply(lambda x: x['player'])
    df['store_name'] = df['store'].apply(lambda x: x['title'])
    df = df.drop('name', axis=1)
    df = df.drop('item', axis=1)
    df = df.drop('store', axis=1)

    df.to_sql('items', con=db, index=False, if_exists='replace')

    r = requests.get('https://api.prontera.ru/api/v3/ru_prime/sold_history/?offset=0&limit=1')
    info = json.loads(r.content)
    limit = info['count'] + 100
    link = 'https://api.prontera.ru/api/v3/ru_prime/sold_history/?offset=0&limit=' + str(limit)

    r = requests.get(link)
    info = json.loads(r.content)

    df = pd.DataFrame.from_dict(info['results'], orient='columns')
    df = df[['player']]
    df['id'] = df['player'].apply(lambda x: x['id'])
    df['seller_name'] = df['player'].apply(lambda x: x['name'])
    df = df.drop('player', axis=1)
    df = df.drop_duplicates()

    df.to_sql('sellers', con=db, index=False, if_exists='replace')

    global COUNT

    r = requests.get('https://api.prontera.ru/api/v3/ru_prime/item_history/?offset=0&limit=1')
    info = json.loads(r.content)
    limit = info['count'] - COUNT
    COUNT = info['count']

    if limit == 0:
        return

    link = 'https://api.prontera.ru/api/v3/ru_prime/item_history/?offset=0&limit=' + str(limit)

    r = requests.get(link)
    info = json.loads(r.content)

    df = pd.DataFrame.from_dict(info['results'], orient='columns')
    df = df[['id', 'player', 'item', 'amount', 'price', 'last_seen']]
    df['seller_id'] = df['player'].apply(lambda x: x['id'])
    df['seller_name'] = df['player'].apply(lambda x: x['name'])
    df['item_id'] = df['item'].apply(lambda x: x['id'])
    df = df.drop('player', axis=1)
    df = df.drop('item', axis=1)

    df.to_sql('purchases', con=db, index=False, if_exists='append')


async def get_top_sellers(id, t_start, t_end, amount, t):
    purchases = cur.execute('SELECT seller_name, amount, price, last_seen FROM purchases WHERE item_id = (?)',
                            [id]).fetchall()
    df = pd.DataFrame(purchases, columns=['player', 'amount', 'price', 'last_seen'])

    df['now'] = datetime.datetime.now().replace(tzinfo=datetime.timezone.utc)

    df['last_seen'] = (pd.to_datetime(df['now']) - pd.to_datetime(df['last_seen'])).dt.round("D")
    df = df.drop('now', axis=1)

    df['last_seen'] = df['last_seen'].apply(lambda x: t_start <= x.days <= t_end)
    df = df[df['last_seen'] == True].reset_index()

    df = df.drop('index', axis=1)
    df = df.drop('last_seen', axis=1)

    df = df.groupby('player').agg({'amount': 'sum', 'price': 'mean'}).reset_index()

    if t in {'price', 'player'}:
        df = df.sort_values(by=[t]).reset_index().drop('index', axis=1)
        df['price'] = df['price'].apply(lambda x: f'{x:.0f}')
    elif t == 'amount':
        df = df.sort_values(by=[t], ascending=False).reset_index().drop('index', axis=1)
        df['price'] = df['price'].apply(lambda x: f'{x:.0f}')

    return df.head(amount)


async def handle_response(user_message):
    if user_message[:7] == 'ro item':
        id = int(user_message.split()[-1])
        items = cur.execute(
            'SELECT item_name, store_name, amount, price, x, y FROM items WHERE item_id = (?) ORDER BY price',
            [id]).fetchall()

        df = pd.DataFrame(items, columns=['name', 'store', 'amount', 'price', 'x', 'y'])

        return '`' + df.to_markdown() + '`'

    if user_message[:9] == 'ro seller':
        name = user_message.split()[-1]
        id = cur.execute('SELECT id FROM sellers WHERE seller_name = (?)', [name]).fetchone()[0]

        items = cur.execute(
            'SELECT item_name, store_name, amount, price, x, y FROM items WHERE store_id = (?) ORDER BY item_name',
            [id]).fetchall()

        df = pd.DataFrame(items, columns=['name', 'store', 'amount', 'price', 'x', 'y'])

        return '`' + df.to_markdown() + '`'

    if user_message[:14] == 'ro top sellers':
        tmp = user_message.split()
        t = tmp[-5]
        t_start = int(tmp[-4])
        t_end = int(tmp[-3])
        amount = int(tmp[-2])
        id = int(tmp[-1])
        df = await get_top_sellers(id, t_start, t_end, amount, t)

        return '`' + df.to_markdown() + '`'

    if user_message[:15] == 'ro lowest price':
        id = int(user_message.split()[-1])
        item = cur.execute(
            'SELECT item_name, store_name, amount, price, x, y FROM items WHERE item_id = (?) ORDER BY price LIMIT 1',
            [id]).fetchall()

        df = pd.DataFrame(item, columns=['name', 'store', 'amount', 'price', 'x', 'y'])

        return '`' + df.to_markdown() + '`'


async def send_message(message, user_message):
    try:
        response = await handle_response(user_message)
        await message.channel.send(response)
    except Exception as e:
        print(e)


def run_discord_bot():
    TOKEN = '<your token>'
    client = discord.Client(intents=discord.Intents.all())

    @client.event
    async def on_ready():
        print(f'{client.user} is running')
        limit = 3000  # cnt of last purchases to load (300000 - all time (1.5 years), ~20000 - 1 month)
        await load_item_history(limit)
        await update()  # update item_history only if there is a new purchase
        scheduler = AsyncIOScheduler()
        scheduler.add_job(update, 'interval', minutes=5)
        scheduler.start()

    @client.event
    async def on_message(message):
        username = str(message.author).split('#')[0]
        user_message = message.content
        print(f'{username}: {user_message}')

        if message.author == client.user:
            return

        await send_message(message, user_message)

    client.run(TOKEN)


run_discord_bot()
