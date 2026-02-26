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
                    let label = document.createElement("span");
                    label.textContent = String.fromCharCode(65 + j);
                    label.classList.add("lower");
                    tile.appendChild(label);
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
            if (j ==0){
                let label = document.createElement("span");
                label.textContent = i + 1;
                label.classList.add("leftmost");
                tile.appendChild(label);
            }
            tile.addEventListener('click',function(){
                onTileClicked(this.dataset.row,this.dataset.col);
            });
            board.append(tile);
        }
    }
    console.log("board initalised!");
})

const selectedPiece = []; // can be used when pieces move
const availablePreMoves = [];
async function onTileClicked(row,col){
    let piece = pieceArray[row][col];
    console.log(pieceArray);
    //handles both highlighting and clearing selected tile. dosent require backend verification

    //sends selected tile(in string form into back end. response will be in string form with format xy& where x y are coordinates and & is to denote a seperate tile.
    // both movement and capture logic is within the response, they are sperated by a 'c'which will denote a change in logic (and css required))
    let url = `/tile-clicked?row=${row}&col=${col}`
        + `&pieceType=${piece ? piece.type : "None"}&colorType=${piece ? piece.color : "None"}`;
    const response = await fetch(url);
    const text = await response.text();

    console.log(text);
    if(text != "None"){
        if(selectedPiece.length == 0){
            checkSelected(row,col,piece);
            generatePreMoves(text);
        }
        else{
            if (selectedPiece[0][0] == row && selectedPiece[0][1] == col){
                clearPreMoves();
            }
            else if (availablePreMoves.some(([r,c]) => r == row && c == col)){
                movePiece(row,col);
                clearPreMoves();
            }
            else{
                clearPreMoves();
                checkSelected(row, col, piece);
                generatePreMoves(text);
            }
        }
    }
    if(text == "None"){
        if(pieceArray[row][col] != null){
            checkSelected(row,col,pieceArray[row][col]);
        }
        if(selectedPiece.length != 0 && availablePreMoves.some(([r,c]) => r == row && c == col)){
            movePiece(row,col);
            clearPreMoves();
        }
        else{
            clearPreMoves();
        }
    }
}

async function movePiece(row,col){
    let url = `/move-piece?pieces=`;
    console.log(pieceArray);
    let [originalRow,originalCol] = selectedPiece[0];
    url += createMovePieceString(originalRow,originalCol,row,col);
    console.log(url);
    const response = await fetch(url);
    let text = await response.text();

    if(text != "validated"){
        console.warn("backend Error");
        return;
    }

    let originalTile = document.querySelector(`[data-row="${originalRow}"][data-col="${originalCol}"]`);
    let newTile = document.querySelector(`[data-row="${row}"][data-col="${col}"]`);
    let capturedImg = newTile.querySelector('img');
    if(capturedImg) capturedImg.remove();
    newTile.appendChild(originalTile.querySelector('img'));

    pieceArray[row][col] = pieceArray[originalRow][originalCol];
    pieceArray[row][col].position = [row, col];
    pieceArray[originalRow][originalCol] = null;
}

function createMovePieceString(originalRow,originalCol,newRow,newCol){
    let pieceString = "";
    let originalPiece = pieceArray[originalRow][originalCol];
    if (originalPiece == null) {
        console.warn("error moving a piece that dosent exist")
        return;
    }
    pieceString += `${originalRow}${originalCol}${typeToText[originalPiece.type]}${colorToText[originalPiece.color]}`

    let newPiece = pieceArray[newRow][newCol];
    let isNewPiece = newPiece != null;

    pieceString += `${newRow}${newCol}${isNewPiece ? typeToText[newPiece.type] : 'o'}${isNewPiece ? colorToText[newPiece.color] : 'o'}`;
    
    return pieceString
}
//ts scuffed as fuck, if it works it works type shit o(n) time though
function generatePreMoves(text){
    //text comes in the form of xy&xy&cxy&xy&... where xy == coords & = seperation between tiles and c = set flag from preMoves to preCaptures
    let preMoveTile = '';
    let isGeneratingPreMove = true;
    for(let s = 0; s < text.length ; s++){
        if (text[s] == "c"){
            isGeneratingPreMove = false;
            continue;
        }
        if (text[s] == '&'){
            let row = parseInt(preMoveTile[0]);
            let col = parseInt(preMoveTile[1]);
            let tile = document.querySelector(`[data-row="${row}"][data-col="${col}"]`);
            if (isGeneratingPreMove){
                let preMoveSpan = document.createElement('span')
                preMoveSpan.classList.add('preMove');
                tile.append(preMoveSpan);
            }
            if(!isGeneratingPreMove){
                if(pieceArray[row][col] != null){
                    let preCaptureSpan = document.createElement('span');
                    preCaptureSpan.classList.add('preCapture');
                    tile.append(preCaptureSpan);
                }
                else{
                    preMoveTile = '';
                    continue;
                }
            }
            availablePreMoves.push([parseInt(preMoveTile[0]),parseInt(preMoveTile[1])]);
            preMoveTile = '';
        }
        else{
            preMoveTile += text[s];
        }
    }
}

function checkSelected(row,col,piece){
    let tile = document.querySelector(`[data-row="${row}"][data-col="${col}"]`);
    if(piece != null){
        selectedPiece.push([row,col]);
        tile.classList.add("selected");
    }
}

function clearSelected(){
    if(selectedPiece.length == 0) return;
    let [row,col] = selectedPiece[0];
    let tile = document.querySelector(`[data-row="${row}"][data-col="${col}"]`);
    tile.classList.remove('selected');
    selectedPiece.length = 0;
}

function generateImageText(piece){
    let imageString = "static/images/vintage/"
    imageString += piece.color =="White" ? "w" : "b";
    imageString += typeToText[piece.type];
    imageString += ".png";
    return imageString;
}

function clearPreMoves(){
    clearSelected();
    if(availablePreMoves.length == 0) return;
    for(let i = 0; i < availablePreMoves.length ; i++){
        let [row,col] = availablePreMoves[i];
        if (row == -1 && col == -1){
            console.log("Piece does not exist");
            continue;
        }
        let tile = document.querySelector(`[data-row="${row}"][data-col="${col}"]`);
        tile.querySelectorAll('.preMove, .preCapture').forEach(t => t.remove());
    }
    availablePreMoves.length = 0;
}

const colorToText = {
    'White' : 'w',
    'Black' : 'b'
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