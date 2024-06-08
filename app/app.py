from flask import Flask, request

app = Flask(__name__)

@app.route("/")
def entry_point():
    return "Index Page"

@app.route("/hello")
def hello():
    return "Hello World"

@app.route("/login", methods=["GET", "POST"])
def login():
    if request.method == "POST":
        return "Masuk pak Eko"
    else:
        return "Ga masuk om"
    
@app.route("/sensor/data", methods="POST")
def sensor():
    return {
        "temperature": 30,
        "kelembapan": 1,
        "timestamp":"09:09:30"
    }

if __name__ == "__main__":
    app.run(debug=True)
