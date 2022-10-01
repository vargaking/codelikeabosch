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

await fetch("http://localhost:5000/testing")
    .then((response) => response.json())
    .then((data) => {
        console.log(data);
        movementData = [
          [
            { x: 1, y: 5, id: 1, type: "car" },
            { x: -1, y: -5, id: 2, type: "car" },
          ],
          [
            { x: 4, y: 5, id: 1, type: "car" },
            { x: 0, y: -5, id: 2, type: "car" },
          ],
        ];
    });

const managerr = new THREE.LoadingManager();

managerr.onLoad = () => {
  console.log("loaded");
  document.getElementById("loadingScreen").style.display = "none";
  run();
};

var view = new Main(managerr);

function run() {
  
  let mainCar = new SceneObject(0, 0, 0, "main");
  mainCar.getMainObject(view.scene);
  //view.scene.add(mainCar.getMainObject(view.scene));
  
  let index = 0;
  let element;
  let timmer = setInterval(() => {
    if (index < 10) {
      try {
      element = movementData[index];

      let timestamp = element;
      view.objects = [];
      console.log(timestamp);
      timestamp.forEach((object) => {
        // check if the object is new or not
        let objectInScene = view.prevObjects.find((sceneObject) => {
          return sceneObject.id === object.id;
        });

  
        if (!objectInScene) {
          // if the object is new, create a new object and add it to the scene
          
          if (object.type === "car" && (object.x != 0 || object.y != 0)) {
            let newObject = new SceneObject(
              object.id,
              object.x,
              object.y,
              object.type
            );

            const currentObj = view.carObj.clone();
            currentObj.position.x = object.x;
            currentObj.position.y = .45;
            currentObj.position.z = object.y;
            currentObj.name = object.id;
            view.scene.add(currentObj);
            view.objects.push(newObject);
            console.log("new car", view.objects);
          }
        } else {
          console.log("object already in scene");
          // if the object is not new, update the position of the object
          let objectInScene = view.scene.getObjectByName(object.id);
          //objectInScene.position.x = object.x;
          //objectInScene.position.z = object.y;

          console.log(object.x - objectInScene.position.x, "delta x");

          let dx = object.x - objectInScene.position.x;
          console.log(dx, "dx");

          let moddedObject = new SceneObject(
            object.id,
            object.x,
            object.y,
            object.type,
            dx,
            object.y - objectInScene.position.z
          );

          console.log(moddedObject.deltaX);

          view.objects.push(moddedObject);
          console.log(view.objects);

        }
        //console.log(object);
      });
      view.prevObjects = view.objects;
    } catch (error) {
      clearInterval(timmer);
    }
    }
    index++;

  }, 1000);
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
