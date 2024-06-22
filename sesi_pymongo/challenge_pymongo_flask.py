import os
import pymongo
from flask import Flask, request
from pymongo.results import InsertManyResult
from dotenv import load_dotenv, dotenv_values

load_dotenv()

app = Flask(__name__)

# Load .env file with your configs that you've get from Mongodb atlas
password = os.getenv("password")
username = os.getenv("username")
host = os.getenv("host")
uri = f"mongodb+srv://{username}:{password}@{host}"

client = pymongo.MongoClient(uri)
db = client.challenge1
myCollection = db.sensor

def kirim_data(temperature, kelembapan):
    sensor = {'kelembapan':kelembapan, 'temperature':temperature}
    results = myCollection.insert_many([sensor])
    print(results.inserted_ids)

@app.route('/', methods= ["GET"])
def hello_world():
    return "Hello World"

@app.route('/sensor1', methods= ["POST"]) # type: ignore
def data():
    temperature = request.args.get("temperature")
    kelembapan = request.args.get("kelembapan")

    if temperature is not None:
        temperature = float(temperature)
    if kelembapan is not None:
        kelembapan = float(kelembapan)

    #mengirim data sensor ke db
    kirim_data(temperature=temperature, kelembapan=kelembapan)

    return f'temperature: {temperature}, kelembapan: {kelembapan}'

if __name__ == "__main__":
    app.run(host="0.0.0.0", debug=True)