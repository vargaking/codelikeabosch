from flask import Flask, request
from flask_cors import CORS
import lib
import pandas
import json

app = Flask(__name__)
CORS(app)

@app.route('/')
def root():
    return "The recordings should be placed into folder 'csvfiles'."

@app.route('/view/<path:data>')
def testing(data):

    fileNames = ["Group_340", "Group_342",
                 "Group_343", "Group_349", "Group_416"]

    fileValues = []
    for fileName in fileNames:
        df = pandas.read_csv(f'csvfiles/{data}/{fileName}.csv')
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

    # simulating ticks and retrieving snapshots every 0.1 sec
    # c++ backend
    snapshots = lib.testing(fileValues)

    return snapshots
