from flask import Flask, request
import lib
import pandas

app = Flask(__name__)


@app.route('/testing')
def testing():
    fileNames = ["Group_340", "Group_342",
                 "Group_343", "Group_349", "Group_416"]

    fileValues = []
    for fileName in fileNames:
        df = pandas.read_csv(f'csvfiles/test1/{fileName}.csv')
        keys = []
        longvalues = []

        for name in df.columns:
            keys.append(name)

        for index, row in df.iterrows():
            value = []
            for key in keys:
                value.append(row[key])
            longvalues.append(value)

        fileValues.append(longvalues)

    return str(fileValues)


@app.route("/")
def response():
    return f"<p>{ lib.test() }</p>"
