# Minesweeper (Flask + C)

這個範例專案使用：
- 前端/路由：Python Flask
- 遊戲邏輯：C 語言動態函式庫
- 溝通方式：Python `ctypes` 呼叫 C 函式

## 需求
- macOS 或 Linux
- Python 3.9+
- gcc

## 啟動方式
```bash
cd minesweeper
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
python app.py
```

開啟瀏覽器到 `http://127.0.0.1:5000`

## API
- `POST /api/new`：建立新局（rows, cols, mines）
- `POST /api/reveal`：開格（row, col）
- `POST /api/flag`：插旗（row, col）
