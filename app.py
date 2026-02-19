from flask import Flask, render_template
import requests

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/click-button')
def click_button():
    # Send HTTP request to C++ backend
    response = requests.post('http://localhost:8080/button-clicked')
    return f"C++ responded: {response.text}"

@app.route('/tile-clicked')
def tile_clicked():
    return

if __name__ == '__main__':
    app.run(port=5000, debug=True)
