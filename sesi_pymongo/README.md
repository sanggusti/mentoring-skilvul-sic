# How to run the script in these folder

Basically all you need is to write your own `.env` file since it's a basic practice of storing credentials in your projects and to make it more reproducible and make your token to be not accessible to bad actors.

To install several necessities

```bash
pip install python-dotenv
```

Load your `.env` file with your `password`, `username` and `host` that are hosting your Mongo Atlas Cluster

```text
password = "your-password"
username = "your-username"
host = "the-long-ass-host-that-ive-separated-for-you"
```

Then basically just run the file that you need to run after writing the `.env` file:

```bash
python challenge_pymongo_flask.py
```
