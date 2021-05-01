# File transfer by socket multicast

###### tags: `computer communication networks` `lab2`

### 簡介
本次練習將利用`multicast`傳送檔案，在`server`端同時傳檔案給三個`client`端。
* 紀錄`server`與`client`連線及傳輸檔案過程。
    * 須包含傳輸的畫面（`4`個`terminal`）。
    * `client`和`server`須包含接收到的檔案大小。
---
### 開發環境
* OS：Ubuntu 20.04.1 LTS (虛擬機)
* CPU：Intel® Core™ i5-1030NG7 CPU @ 1.10GHz × 4
* Memory：4 GiB
* Programming Language：C17
---
### 使用說明
* 編譯：
    ```bash=
    # for server
    gcc -o server multicast_server.c
    ```
    ```bash=
    # for client
    gcc -o client multicast_client.c
    ```
* 執行：
    ```bash=
    # for client
    ./client
    ```
    ```bash=
    # for server
    ./server <filename>
    ```
---
### 執行結果

![](https://i.imgur.com/594HFvX.png)

* 左ㄧ為`server`端（`send`），依序三個為`client`端（`receive`）。
* 紀錄訊息包含：
    * 建立連線的過程。
    * 檔案大小。
* 接收端的檔名為：`receive.txt`。
* **由於`multicast`是使用`UDP`作為傳輸的協定，因此在傳遞的過程會有封包丟失，所以接收端（`client`）收到的檔案大小少於傳送端（`server`）所送出的檔案。**


