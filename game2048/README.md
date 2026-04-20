# 2048 (Flask + C)

前端使用 Flask，核心邏輯由 C 動態函式庫提供，Python 透過 `ctypes` 呼叫。

## 啟動

```bash
cd game2048
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
python app.py
```

開啟 `http://127.0.0.1:5000`

## 控制

- 鍵盤方向鍵：移動方塊
- 頁面按鈕：上/下/左/右
- 新遊戲：重開一局
