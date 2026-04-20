const boardEl = document.getElementById("board");
const statusEl = document.getElementById("status");
const rowsEl = document.getElementById("rows");
const colsEl = document.getElementById("cols");
const minesEl = document.getElementById("mines");
const newBtn = document.getElementById("new-game");

let current = null;

function setStatus(data) {
  if (data.win) {
    statusEl.textContent = "恭喜，已通關";
  } else if (data.game_over) {
    statusEl.textContent = "踩到地雷，遊戲結束";
  } else {
    statusEl.textContent = "進行中";
  }
}

function render(data) {
  current = data;
  boardEl.style.gridTemplateColumns = `repeat(${data.cols}, 32px)`;
  boardEl.innerHTML = "";
  setStatus(data);

  for (let r = 0; r < data.rows; r += 1) {
    for (let c = 0; c < data.cols; c += 1) {
      const cell = data.board[r][c];
      const btn = document.createElement("button");
      btn.className = "cell";
      btn.textContent = cell.text;
      if (cell.value !== -3 && cell.value !== -2) btn.classList.add("revealed");
      if (cell.value === -1) btn.classList.add("mine");

      btn.addEventListener("click", () => reveal(r, c));
      btn.addEventListener("contextmenu", (e) => {
        e.preventDefault();
        flag(r, c);
      });
      boardEl.appendChild(btn);
    }
  }
}

async function post(url, payload) {
  const response = await fetch(url, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(payload),
  });
  return response.json();
}

async function newGame() {
  const rows = Number(rowsEl.value);
  const cols = Number(colsEl.value);
  const mines = Number(minesEl.value);
  if (mines >= rows * cols) {
    statusEl.textContent = "地雷數需小於總格數";
    return;
  }
  const data = await post("/api/new", { rows, cols, mines });
  render(data);
}

async function reveal(row, col) {
  if (!current || current.game_over || current.win) return;
  const data = await post("/api/reveal", { row, col });
  render(data);
}

async function flag(row, col) {
  if (!current || current.game_over || current.win) return;
  const data = await post("/api/flag", { row, col });
  render(data);
}

newBtn.addEventListener("click", newGame);
newGame();
