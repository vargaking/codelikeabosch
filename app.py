from tracemalloc import Snapshot
from flask import Flask, request
from flask_cors import CORS
import lib
import pandas
import json

app = Flask(__name__)
CORS(app)

@app.route('/testing')
def testing():
    print("asd")
    
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

    snapshots = lib.testing(fileValues)
    
    
    """# load ft_test.json into snapshots variable
    with open('ft_test.json') as f:
        snapshots = json.load(f)"""

    return snapshots


@app.route('/testing2')
def testing2():
    # load ft_test.json into snapshots variable
    with open('ft_test.json') as f:
        snapshots = json.load(f)

    return snapshots