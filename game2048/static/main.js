const boardEl = document.getElementById("board");
const scoreEl = document.getElementById("score");
const statusEl = document.getElementById("status");
const newBtn = document.getElementById("new-game");
const actionBtns = document.querySelectorAll(".actions button");

let current = null;

function tileClass(v) {
  if (v === 0) return "tile tile-0";
  return `tile tile-${v}`;
}

function setStatus(data) {
  if (data.win) {
    statusEl.textContent = "達成 2048，恭喜！";
    return;
  }
  if (data.game_over) {
    statusEl.textContent = "沒有可移動步數，遊戲結束。";
    return;
  }
  statusEl.textContent = "進行中";
}

function render(data) {
  current = data;
  scoreEl.textContent = `Score: ${data.score}`;
  setStatus(data);
  boardEl.innerHTML = "";
  data.board.flat().forEach((v) => {
    const tile = document.createElement("div");
    tile.className = tileClass(v);
    tile.textContent = v === 0 ? "" : String(v);
    boardEl.appendChild(tile);
  });
}

async function post(url, payload = {}) {
  const response = await fetch(url, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(payload),
  });
  return response.json();
}

async function newGame() {
  const data = await post("/api/new");
  render(data);
}

async function move(direction) {
  if (!current || current.game_over) return;
  const data = await post("/api/move", { direction });
  render(data);
}

newBtn.addEventListener("click", newGame);
actionBtns.forEach((btn) => {
  btn.addEventListener("click", () => move(Number(btn.dataset.dir)));
});

window.addEventListener("keydown", (e) => {
  if (e.key === "ArrowUp") move(0);
  if (e.key === "ArrowDown") move(1);
  if (e.key === "ArrowLeft") move(2);
  if (e.key === "ArrowRight") move(3);
});

newGame();
