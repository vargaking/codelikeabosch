# codelikeabosch

For testing the project we strongly recommend to use a Unix environment (Linux, Mac OS) and a Python virtual environment.

First you have to install the dependencies with `pip install -r requirements.txt`.
After that, you should be able to start the simulation server by running `make run` in the repository root, which will start the server on port 5000.

If you want to add new csv files, you have to add them to new directory inside ´csvfiles´. Then by reaching `localhost:5000/view/<foldername>`, you will be able to generate the output of our program in. It will take up to a minute, depending on the input length. It is in JSON format, and can be utilized directly from this endpoint, however it is also saved next to the CSV files, so that it does not need to be recomputed every time. 

You can also visualize it using our frontend. First you have to cd to the `frontend` directory and run `npm install`. After that you can run `npm run dev` and you will be able to run the visualization on `localhost:5173` (or on whatever the port it gives). When you enter the page, you'll be prompted to enter the CSV's directory name, from which it will read the JSON data, and the visualization starts right after that.