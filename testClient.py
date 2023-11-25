import requests
from tkinter.colorchooser import askcolor

url = "http://172.20.10.9/"

running = True
while running:
    col = askcolor()[0]
    print(col)
    if not col:
        print("False")
        running = False
        continue

    data = f"{col[0]},{col[1]},{col[2]}"
    print(data)
    try:
        answer = requests.post(url, data=data, timeout=1)
    except requests.exceptions.ConnectionError:
        pass
    except requests.exceptions.ConnectTimeout:
        pass
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
