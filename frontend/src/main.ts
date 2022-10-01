// import Main class from ts/three.ts

import { Main } from './ts/three';
import { SceneObject } from './ts/objects';
import * as THREE from "three";


// create an object for the movement data

// level 1: timestamp
// level 2: object id
// level 3: x, y, type


const movementData = [
  {
    objects: [
      {
        id: 0,
        x: 0,
        y: 0,
        type: 'main'
      },
      {
        id: 1,
        x: 1,
        y: 2,
        type: 'car'
      },
      {
        id: 2,
        x: -1,
        y: -1,
        type: 'car'
      },
    ]
  },
  {
    objects: [
      {
        id: 0,
        x: 0,
        y: 0,
        type: 'main'
      },
      {
        id: 1,
        x: 1,
        y: 1,
        type: 'car'
      },
      {
        id: 2,
        x: -1,
        y: -2,
        type: 'car'
      },
    ]
  },
  {
    objects: [
      {
        id: 0,
        x: 0,
        y: 0,
        type: 'main'
      },
      {
        id: 1,
        x: 1,
        y: 0,
        type: 'car'
      },
      {
        id: 2,
        x: -1,
        y: -3,
        type: 'car'
      },
    ]
  },
  {
    objects: [
      {
        id: 0,
        x: 0,
        y: 0,
        type: 'main'
      },
      {
        id: 1,
        x: 1,
        y: -.5,
        type: 'car'
      },
      {
        id: 2,
        x: -1,
        y: -3.5,
        type: 'car'
      },
    ]
  },
];

let view = new Main();

let clock = new THREE.Clock();

// initialize the scene with the object at the first timestamp
movementData[0].objects.forEach((object, index) => {
  let sceneObject = new SceneObject(object.id, object.x, object.y, object.type);
  view.objects.push(sceneObject);

  // add the object to the previous objects
  view.prevObjects.push(sceneObject);
  view.renderObjects.push(sceneObject.getCarObject());

  view.scene.add(view.renderObjects[index]);

  console.log(sceneObject)

});

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
    index = 0;
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