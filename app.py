from flask import Flask
import lib

app = Flask(__name__)

@app.route("/")
def response():
    return f"<p>{ lib.test() }</p>"