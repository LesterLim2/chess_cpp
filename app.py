from flask import Flask, render_template ,request
import requests
#THIS ENTIRE THING NEEDS TO BE GONE

app = Flask(__name__)
CPP_BASE = 'http://127.0.0.1:8080'
session = requests.Session()

@app.route('/')
def index():
    return render_template('board.html')

@app.route('/tile-clicked',methods = ["GET"])
def tile_clicked():
    tile_parameters = {
        'row' : request.args.get('row'),
        'col' : request.args.get('col'),
        'pieceType' : request.args.get('pieceType'),
        'colorType' : request.args.get('colorType'),
    }
    response = session.get(f'{CPP_BASE}/tile-clicked', params=tile_parameters)
    return response.text

@app.route('/move-piece',methods = ['GET'])
def move_piece():
        print(request.args.get('pieces'))
        response = session.get(f'{CPP_BASE}/move-piece?pieces={request.args.get('pieces')}')
        return response.text
if __name__ == '__main__':
    app.run(port=5000, debug=True)
