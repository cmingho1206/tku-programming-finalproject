from __future__ import annotations

import csv
from dataclasses import dataclass
from pathlib import Path
from typing import List

from flask import Flask, redirect, render_template, request, url_for


BASE_DIR = Path(__file__).resolve().parent
DATA_DIR = BASE_DIR / "data"
DATA_FILE = DATA_DIR / "expenseData.csv"


@dataclass
class Expense:
    date: str
    shop: str
    amount: float


def ensure_data_file() -> None:
    DATA_DIR.mkdir(parents=True, exist_ok=True)
    DATA_FILE.touch(exist_ok=True)


def read_expenses() -> List[Expense]:
    ensure_data_file()
    expenses: List[Expense] = []
    with DATA_FILE.open("r", encoding="utf-8", newline="") as fp:
        reader = csv.reader(fp)
        for row in reader:
            if len(row) != 3:
                continue
            try:
                expenses.append(
                    Expense(
                        date=row[0].strip(),
                        shop=row[1].strip(),
                        amount=float(row[2]),
                    )
                )
            except ValueError:
                continue
    return expenses


def append_expense(expense: Expense) -> None:
    ensure_data_file()
    with DATA_FILE.open("a", encoding="utf-8", newline="") as fp:
        writer = csv.writer(fp)
        writer.writerow([expense.date, expense.shop, f"{expense.amount:.2f}"])


app = Flask(__name__)


@app.route("/")
def index():
    expenses = read_expenses()
    total = sum(item.amount for item in expenses)
    return render_template("index.html", expenses=expenses, total=total)


@app.route("/add", methods=["POST"])
def add_expense_route():
    date = request.form.get("date", "").strip()
    shop = request.form.get("shop", "").strip()
    amount_text = request.form.get("amount", "").strip()

    if not date or not shop or not amount_text:
        return redirect(url_for("index"))

    try:
        amount = float(amount_text)
    except ValueError:
        return redirect(url_for("index"))

    append_expense(Expense(date=date, shop=shop, amount=amount))
    return redirect(url_for("index"))


if __name__ == "__main__":
    app.run(debug=True)
