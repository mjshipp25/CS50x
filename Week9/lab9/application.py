import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

MONTHS = {"january":"1",
    "febuary":"2",
    "march":"3",
    "april":"4",
    "may":"5",
    "june":"6",
    "july":"7",
    "august":"8",
    "september":"9",
    "october":"10",
    "november":"11",
    "december":"12"
}

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        name = request.form.get("name")
        month = request.form.get("month")
        month = month.lower()
        day = request.form.get("day")

        # Converts text month to number
        if month in MONTHS.keys():
            month = MONTHS[month]

        db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day)
        return redirect("/")

    else:
        birthdays = db.execute("SELECT * FROM birthdays")
        return render_template("index.html", birthdays = birthdays)


@app.route("/remove", methods=["GET"])
def remove():
    ied = request.args.get("remove")
    db.execute("DELETE FROM birthdays WHERE id IS ?", ied)
    return redirect("/")