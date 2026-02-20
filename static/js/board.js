function Piece(color,type,position){
    this.color = color;
    this.type = type;
    this.position = position;
}

const pieceArray = Array.from({length : 8}, () => Array(8).fill(null));
const backRowsArray = ["Rook","Knight","Bishop","Queen","King","Bishop","Knight","Rook"];
document.addEventListener('DOMContentLoaded',function(){
    const board = document.getElementById('chessBoard');
    for(let i = 7; i >= 0; i--){
        for(let j = 0; j < 8;j++){
            const tile = document.createElement('div');
            tile.classList.add("tile");
            tile.classList.add((i + j) % 2 == 0 ? "dark" : "light");
            tile.dataset.row = i;
            tile.dataset.col = j;
            switch(i){
                case 0:
                    placePiece(tile, "White", backRowsArray[j], i, j);
                    break;
                case 1:
                    placePiece(tile, "White", "Pawn", i, j);
                    break;
                case 6:
                    placePiece(tile, "Black", "Pawn", i, j);
                    break;
                case 7:
                    placePiece(tile, "Black", backRowsArray[j], i, j);
                    break;
            }
            tile.addEventListener('click',function(){
                onTileClicked(this.dataset.row,this.dataset.col);
            });
            board.append(tile);
        }
    }
})

const selectedPieces = [];
async function onTileClicked(row,col){
    let piece = pieceArray[row][col];
    checkSelected(row,col,piece);

    let url = `/tile-clicked?row=${row}&col=${col}`
        + `&pieceType=${piece ? piece.type : "None"}&colorType=${piece ? piece.color : "None"}`;
    const response = await fetch(url);
    const text = await response.text();
    console.log(text);
}

function checkSelected(row,col,piece){
    let tile = document.querySelector(`[data-row="${row}"][data-col="${col}"]`);
    //there is a typeError and i know why but it seems like such a huge hassle to fix LOL, it still works LMAO
    if(selectedPieces.length == 0 && piece != null){
        console.log(selectedPieces);
        selectedPieces.push([row,col]);
        tile.classList.add("selected")
    }
    else{
        let [x,y] = selectedPieces[0];
        if ((x == row) && (y == col)){
            //if double click the same piece
            selectedPieces.pop();
            tile.classList.remove("selected");
        }
        else{
            selectedPieces.pop();
            let originalTile = document.querySelector(`[data-row="${x}"][data-col="${y}"]`);
            originalTile.classList.remove("selected");
            
            selectedPieces.push([row,col]);
            tile.classList.add("selected");
        }
        console.log(selectedPieces);
    }
    
}

const typeToText = {
    'Pawn' : 'p',
    'Knight' : 'n',
    'Bishop' : 'b',
    'Rook' : 'r',
    'Queen' : 'q',
    'King' : 'k'
}

function placePiece(tile, color, type, row, col){
    pieceArray[row][col] = new Piece(color, type, [row, col]);
    let img = document.createElement('img');
    img.src = generateImageText(pieceArray[row][col]);
    tile.append(img);
}

function generateImageText(piece){
    let imageString = "static/images/vintage/"
    imageString += piece.color =="White" ? "w" : "b";
    imageString += typeToText[piece.type];
    imageString += ".png";
    return imageString;
}