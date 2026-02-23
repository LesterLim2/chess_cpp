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
    if(text != "None"){
        generatePreMoves(text,row,col);
    }
    if(text == "None" && availablePreMoves != 0 ){
        clearPreMoves();
    }
}

function checkSelected(row,col,piece){
    let tile = document.querySelector(`[data-row="${row}"][data-col="${col}"]`);
    if(selectedPieces.length == 0){
        if(piece!= null){
            console.log(selectedPieces);
            selectedPieces.push([row,col]);
            tile.classList.add("selected");
        }
    }
    else{
        let [x,y] = selectedPieces[0];
        if ((x == row) && (y == col)){
            //on double click remove highlight on piece
            selectedPieces.pop();
            tile.classList.remove("selected");
        }
        else{
            selectedPieces.pop();
            let originalTile = document.querySelector(`[data-row="${x}"][data-col="${y}"]`);
            originalTile.classList.remove("selected");
            
            //if the user clicks on an empty tile when a
            if(pieceArray[row][col] != null){
                selectedPieces.push([row,col]);
                tile.classList.add("selected");
            }
        }
        console.log(selectedPieces);
    }
    
}
function generateImageText(piece){
    let imageString = "static/images/vintage/"
    imageString += piece.color =="White" ? "w" : "b";
    imageString += typeToText[piece.type];
    imageString += ".png";
    return imageString;
}

const availablePreMoves = [];
function generatePreMoves(text){
    let preMoveTile = '';
    if (availablePreMoves.length != 0){
        clearPreMoves();
    }
    for(let s = 0; s < text.length ; s++){
        if (text[s] == '&'){
            let row = parseInt(preMoveTile[0]);
            let col = parseInt(preMoveTile[1]);
            let tile = document.querySelector(`[data-row="${row}"][data-col="${col}"]`);
            if(pieceArray[row][col] == null){
                tile.classList.add('preMove');
            }
            else{
                tile.classList.add('preCapture')
            }
            availablePreMoves.push([parseInt(preMoveTile[0]),parseInt(preMoveTile[1])]);
            preMoveTile = '';
        }
        else{
            preMoveTile += text[s];
        }
    }
}

function clearPreMoves(){
    for(let i = 0 ; i < availablePreMoves.length ; i++){
        let [row,col] =  availablePreMoves[i];
        if (row == -1 && col == -1){
            console.log("Piece does not exist");
            continue;
        }
        let tile = document.querySelector(`[data-row="${row}"][data-col="${col}"]`);
        if(pieceArray[row][col] == null){
            tile.classList.remove('preMove');

        }
        else {
            tile.classList.remove('preCapture');
        }
    }
    availablePreMoves.length = 0;
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