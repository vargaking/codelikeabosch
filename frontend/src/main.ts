// import Main class from ts/three.ts

import { Main } from "./ts/three";
import { SceneObject } from "./ts/objects";
import * as THREE from "three";

// create an object for the movement data

// level 1: timestamp
// level 2: object id
// level 3: x, y, type

// fetch movement data from the localhost:5000/testing

let movementData = [];

function abs(x) {
    return x < 0 ? -x : x;
}

// prompt user for path

let path = prompt("Enter path to data file");

await fetch("http://localhost:5000/view_ft/" + path)
    .then((response) => response.json())
    .then((data) => {
        console.log(data);
        movementData = data;
    });

const managerr = new THREE.LoadingManager();

managerr.onLoad = () => {
    console.log("loaded");
    document.getElementById("loadingScreen").style.display = "none";
    run();
};

var view = new Main(managerr);

function run() {
    //view.scene.add(mainCar.getMainObject(view.scene));

    let index = 0;
    let element;
    let timmer = setInterval(() => {
        if (index < movementData.length) {
            console.log(index, "anyadat index");
            try {
                element = movementData[index];

                if (element.length > 0) {
                    let timestamp = element;
                    view.objects = [];
                    //console.log(view.mainCarObj, "lol");

                    // host car movement
                    /*
                    if (movementData[index - 1].length > 0) {
                        try {
                            let deltaY = abs(movementData[index][0].y) - abs(movementData[index - 1][0].y);
                            if (movementData[index][0].y < movementData[index - 1][0].y) {
                                view.mainCarObj.deltaY -= deltaY;
                            } else {
                                view.mainCarObj.deltaY += deltaY;
                            }

                            console.log(deltaY, "deltaYY");

                            let deltaX = abs(movementData[index][0].x) - abs(movementData[index - 1][0].x);
                            if (movementData[index][0].x < movementData[index - 1][0].x) {
                                view.mainCarObj.deltaX -= deltaX;
                            } else {
                                view.mainCarObj.deltaX += deltaX;
                            }
                            view.mainCarObj.angle = movementData[index][0].angle;
                            view.mainCarObj.deltaX = deltaX;
                            view.mainCarObj.deltaY = deltaY;
                            view.mainCarObj.x = movementData[index][0].x;
                            view.mainCarObj.y = movementData[index][0].y;
                            console.log(view.mainCarObj.deltaY);
                        } catch (e) {
                            console.log(e, index, "error 1");
                            view.mainCarObj.deltaX = 0;
                            view.mainCarObj.deltaY = 0;
                        }
                    */
                    //console.log(timestamp);
                    timestamp.forEach((object) => {
                        //console.log(object)

                        // check if the object is new or not
                        let objectInScene = view.prevObjects.find((sceneObject) => {
                            return sceneObject.id === object.id;
                        });

                        if (!objectInScene) {
                            // if the object is new, create a new object and add it to the scene

                            if (object.type === "car") {
                                let newObject = new SceneObject(object.id, object.angle, object.x, object.y, object.type);

                                const currentObj = view.carObj.clone();
                                currentObj.position.x = object.x;
                                currentObj.position.y = 0.45;
                                currentObj.position.z = object.y;
                                currentObj.name = object.id;
                                view.scene.add(currentObj);
                                view.objects.push(newObject);
                                //console.log("new car", view.objects);
                            } else if (object.type === "truck") {
                                let newObject = new SceneObject(object.id, Math.PI / 2, object.x, object.y, object.type);
								console.log("yey truck")
                                const currentObj = view.truckObj.clone();
                                currentObj.position.x = object.x;
                                currentObj.position.y = .28;
                                currentObj.position.z = object.y;
                                currentObj.name = object.id;
                                view.scene.add(currentObj);
                                view.objects.push(newObject);
                                //console.log("new car", view.objects);
                            } else if (object.type === "bicycle" || object.type === "motorcycle") {
                                let newObject = new SceneObject(object.id, object.angle, object.x, object.y, object.type);
								console.log("yey bike")
                                const currentObj = view.bikeObj.clone();
                                currentObj.position.x = object.x;
                                currentObj.position.y = .7;
                                currentObj.position.z = object.y;
                                currentObj.name = object.id;
                                view.scene.add(currentObj);
                                view.objects.push(newObject);
                                //console.log("new car", view.objects);
                            } else if (object.type === "pedestrian") {
                                let newObject = new SceneObject(object.id, object.angle, object.x, object.y, object.type);
								console.log("yey person")
                                const currentObj = view.pedestrianObj.clone();
                                currentObj.position.x = object.x;
                                currentObj.position.y = .6;
                                currentObj.position.z = object.y;
                                currentObj.name = object.id;
                                view.scene.add(currentObj);
                                view.objects.push(newObject);
                                //console.log("new car", view.objects);
                            }
							else if (object.type === "unknown") {
                                // create a red ball
                                let newObject = new SceneObject(object.id, 0, object.x, object.y, object.type);

                                let newObjectObj = new THREE.SphereGeometry(.3, 8, 4);
                                let newMaterial = new THREE.MeshBasicMaterial({ color: 0xff0000 });
                                let currentObj = new THREE.Mesh(newObjectObj, newMaterial);

                                currentObj.position.x = object.x;
                                currentObj.position.y = 1;
                                currentObj.position.z = object.y;
                                currentObj.name = object.id;
                                view.scene.add(currentObj);
                                view.objects.push(newObject);
                                //console.log("new unknown", view.objects);
                            }

                            /*else if (object.type === "truck") {
						let newObject = new SceneObject(
						object.id,
						object.x,
						object.y,
						object.type
						);

            const currentObj = view.truckObj.clone();
            currentObj.position.x = object.x;
            currentObj.position.y = .28;
            currentObj.position.z = object.y;
            currentObj.name = object.id;
            view.scene.add(currentObj);
            view.objects.push(newObject);
            console.log("new truck", view.objects);
          } else if (object.type === "pedestrian") {
            let newObject = new SceneObject(
              object.id,
              object.x,
              object.y,
              object.type
            );

            const currentObj = view.pedestrianObj.clone();
            currentObj.position.x = object.x;
            currentObj.position.y = .45;
            currentObj.position.z = object.y;
            currentObj.name = object.id;
            view.scene.add(currentObj);
            view.objects.push(newObject);
            console.log("new pedestrian", view.objects);
          }*/
                        } else {
                            console.log("object already in scene");
                            // if the object is not new, update the position of the object
                            let objectInScene = view.scene.getObjectByName(object.id);

                            let dx = object.x - objectInScene.position.x;
                            //console.log(dx, "dx");

                            let moddedObject = new SceneObject(object.id, object.angle, object.x, object.y, object.type, dx, object.y - objectInScene.position.z);

                            //console.log(moddedObject.deltaX);

                            view.objects.push(moddedObject);
                            //console.log(view.objects);
                        }

                        //console.log(object);
                    });

                    // collect objects that are not in the current timestamp
                    let objectsToRemove = view.prevObjects.filter((prevObject) => {
                        let objectInCurrentTimestamp = timestamp.find((object) => {
                            return object.id === prevObject.id;
                        });

                        return !objectInCurrentTimestamp;
                    });

                    // remove the objects that are not in the current timestamp
                    objectsToRemove.forEach((object) => {
                        let objectInScene = view.scene.getObjectByName(object.id);
                        view.scene.remove(objectInScene);
                    });

                    view.prevObjects = view.objects;
                }
            } catch (error) {
                console.log(error, index);
                clearInterval(timmer);
            }
            index++;
        } else {
            console.log("baszodj meg");
            cancelAnimationFrame(view.animationID);
            view.mainCarObj.deltaX = 0;
            view.mainCarObj.deltaY = 0;
        }
    }, 100);
}

/*
// initialize the scene with the object at the first timestamp
movementData[0].forEach((object, index) => {
    let sceneObject = new SceneObject(object.id, object.x, object.y, object.type);
    view.objects.push(sceneObject);

    // add the object to the previous objects
    view.prevObjects.push(sceneObject);
    view.renderObjects.push(sceneObject.getCarObject());

    view.scene.add(view.renderObjects[index]);

    console.log(sceneObject);
});

/*

console.log(view.objects, "objects", view.objects.length);

// set interval to update the scene every 500 ms
let index: number = 1;
const updater = setInterval(() => {
  console.log(index, "index");

  // clear the objects
  view.objects = [];

  // update the scene with the object at the next timestamp
  movementData[index].objects.forEach((object) => {
    let sceneObject = new SceneObject(object.id, object.x, object.y, object.type);
    
    view.objects.push(sceneObject);

  });

  // update the index
  index++;
  
  // if the index is greater than the length of the movement data, stop the interval
  if (index >= movementData.length) {
    clearInterval(updater);
  }

  view.updateScene();

}, 500);

/*
// iterate over the movement data
for (let timestamp in movementData) {
  // iterate over the objects
  for (let object in movementData[timestamp].objects) {
    let sceneObject = new SceneObject(movementData[timestamp].objects[object].id, movementData[timestamp].objects[object].x, movementData[timestamp].objects[object].y, movementData[timestamp].objects[object].type);
    
    // check if the object is already on the scene
    if (objectsOnScene.indexOf(sceneObject) == -1) {
      // add the object to the scene
      view.scene.add(sceneObject.getCarObject());

      objectsOnScene.push(sceneObject);
    }    
  }
}

// create a car object
const car = new SceneObject(0, 0, 0, 'car');
console.log(car.getCarObject());
view.scene.add(car.getCarObject());*/
