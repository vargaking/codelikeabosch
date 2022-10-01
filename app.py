from flask import Flask, request
import lib
import pandas
import json

app = Flask(__name__)


class ObjectSnapshot:
    def __init__(self, obj):
        self.x = obj.get_x()
        self.y = obj.get_y()
        self.type = obj.get_type()
        self.id = obj.get_id()


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

    return snapshots
