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
    fileNames = ["Group_340", "Group_342",
                 "Group_343", "Group_349", "Group_416"]

    fileValues = []
    for fileName in fileNames:
        df = pandas.read_csv(f'csvfiles/test1/{fileName}.csv')
        keys = []
        longvalues = []

        for name in df.columns:
            keys.append(name)

        counter = 0
        for index, row in df.iterrows():
            if (counter == 200):
                break
            counter += 1
            value = []
            for key in keys:
                value.append(row[key])
            longvalues.append(value)

        fileValues.append(longvalues)

    snapshots = lib.testing(fileValues)
    ticks = []
    for tick in snapshots:
        objects = []
        for obj in tick:
            s = ObjectSnapshot(obj)
            objects.append(s.__dict__)
        ticks.append(objects)

    return ticks
