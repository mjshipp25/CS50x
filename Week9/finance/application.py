import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    userID = session["user_id"]
    stocks = db.execute("SELECT * FROM portfolio WHERE id = ? AND shares > 0 ORDER BY price DESC", userID)
    cash = db.execute("SELECT cash FROM users WHERE id = ?", userID)
    cash = cash[0]["cash"]
    return render_template("index.html", stocks=stocks, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # Render buy form if method is GET
    if request.method == "GET":
        return render_template("buy.html")
    else:
        userID = session["user_id"]
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")
        if shares.isalpha() is True:
            return apology("Please enter a positive number of stocks.")
        else:
            shares = float(shares)

            # Checks for valid number of stocks and checks if the shares entered are a positive number
            if shares % 1 != 0 or shares <= 0:
                return apology("Please enter a positive number of stocks.")

        # Checks if a symbol was entered
        if symbol is None:
            return apology("Missing Stock Symbol")

        # Check if the symbol exists
        elif lookup(symbol) is None:
            return apology("No Stock Found")

        # Completes the buy transaction
        else:
            # Add transaction to history
            db.execute("INSERT INTO history (id, symbol, shares, type, name, price) VALUES (?, ?, ?, ?, ?, ?)",
                       userID, symbol, shares, "Bought", lookup(symbol).get("name"), shares * lookup(symbol).get("price"))

            # Add stocks to portfolio
            check = db.execute("SELECT * FROM portfolio WHERE id = ? AND symbol = ?", userID, symbol)
            if len(check) == 0:
                db.execute("INSERT INTO portfolio (id, symbol, shares, price) VALUES (?, ?, ?, ?)",
                           userID, symbol, shares, shares * lookup(symbol).get("price"))
            else:
                shareCount = db.execute("SELECT shares FROM portfolio WHERE id = ? AND symbol = ?", userID, symbol)
                shareCount = shareCount[0]["shares"]
                totShares = shareCount + shares
                totPrice = totShares * lookup(symbol).get("price")
                db.execute("UPDATE portfolio SET shares = ?, price = ? WHERE id = ? AND symbol = ?",
                           totShares, totPrice, userID, symbol)

            # Change cash balance
            oldBalance = db.execute("SELECT cash FROM users WHERE id = ?", userID)
            balance = oldBalance[0]["cash"]
            db.execute("UPDATE users SET cash = ? WHERE id = ?", balance - (shares * lookup(symbol).get("price")), userID)
        return redirect("/")


@app.route("/history")
@login_required
def history():
    userID = session["user_id"]
    stocks = db.execute("SELECT * FROM history WHERE id = ?", userID)
    return render_template("history.html", stocks=stocks)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        company = lookup(symbol)
        if company is None:
            return apology("Stock Not Found")
        else:
            return render_template("quoted.html", company=company)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        # Check if username exists
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if username == "" or password == "":
            return apology("no username or password entered")
        elif password != confirmation:
            return apology("passwords do not match")
        else:
            finPassword = generate_password_hash(password)
            try:
                db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, finPassword)
            except:
                return apology("Username is taken")
            return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    userID = session["user_id"]

    # Render sell form if method is GET
    if request.method == "GET":
        stocks = db.execute("SELECT symbol FROM portfolio WHERE id = ?", userID)
        return render_template("sell.html", stocks=stocks)
    else:
        symbol = request.form.get("symbol").upper()
        shares = int(request.form.get("shares"))
        portfolioShares = db.execute("SELECT symbol FROM portfolio WHERE symbol = ?", symbol)
        portfolioShares = str(portfolioShares[0]["symbol"])
        shareCount = db.execute("SELECT shares FROM portfolio WHERE id = ? AND symbol = ?", userID, symbol)
        shareCount = shareCount[0]["shares"]

        # Check if a symbol was entered
        if symbol is None:
            return apology("Missing Stock Symbol")

        # Check if user owns any of the stock
        elif symbol != portfolioShares:
            return apology("No Shares Owned")

        # Check if the shares entered are a positive number
        elif shares <= 0:
            return apology("Please enter a positive number of stocks.")

        # Check if user has enough stocks in portfolio
        elif shareCount < shares:
            return apology("Not Enough Shares in Portfolio")

        # Completes the sell transaction
        else:
            # Add transaction to history
            db.execute("INSERT INTO history (id, symbol, shares, type, name, price) VALUES (?, ?, ?, ?, ?, ?)",
                       userID, symbol, shares, "Sold", lookup(symbol).get("name"), shares * lookup(symbol).get("price"))

            # Remove stocks from portfolio
            oldShares = db.execute("SELECT shares FROM portfolio WHERE id = ?", userID)
            oldShares = oldShares[0]["shares"]
            db.execute("UPDATE portfolio SET shares = ?, price = ? WHERE id = ? AND symbol = ?",
                       shareCount - shares, (oldShares - shares) * lookup(symbol).get("price"), userID, symbol)

            # Change cash balance
            oldBalance = db.execute("SELECT cash FROM users WHERE id = ?", userID)
            oldBalance = oldBalance[0]["cash"]
            db.execute("UPDATE users SET cash = ? WHERE id = ?", oldBalance + (lookup(symbol).get("price") * shares), userID)
            return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
