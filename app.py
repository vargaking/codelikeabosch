from flask import Flask, request
import lib
import pandas

app = Flask(__name__)

@app.route('/testing')
def testing():
    df = pandas.read_csv('first.csv')
    return f"<p>hello</p>"


@app.route("/")
def response():
    return f"<p>{ lib.test() }</p>"

