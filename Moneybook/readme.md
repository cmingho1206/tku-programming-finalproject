## Moneybook

目前保留兩個版本：

- C 版本：沿用原本 CLI 記帳流程
- Flask 版本：提供網頁前端，後端邏輯對齊 C 版本（新增支出、讀取支出）

## C 版本執行方式

直接執行 `./main`

## Flask 版本執行方式

1. 安裝依賴

```bash
pip install flask
```

2. 啟動服務

```bash
python app.py
```

3. 開啟瀏覽器到：

`http://127.0.0.1:5000`

## 資料檔案

Flask 與 C 版共用同一份資料檔：

`data/expenseData.csv`