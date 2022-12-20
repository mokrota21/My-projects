from selenium import webdriver
from bs4 import BeautifulSoup

test = 'what a beautiful deauwamg'
print(test[:2], test[2:])

driver = webdriver.Chrome()
url = 'https://prontera.ru/ru_prime/item/'

def Get_Item_By_ID(id):
    link = url + id
    driver.get(link)
    html = driver.page_source
    soup = BeautifulSoup(html, 'lxml')
    item = soup.find_all('td', class_="text-start")
    res = -1
    for obj in item:
        info = obj.text
        # print(info)
        if (info[len(info) - 1] == 'z'):
            price = info[:len(info) - 2]
            if res == -1:
                res = price
            else:
                res = min(res, price)
    return res
cont = 'y'
while cont == 'y':
    id = str(input('Enter Item ID: '))
    print(Get_Item_By_ID(id))
    cont = str(input('Continue? y/n '))

driver.quit()
