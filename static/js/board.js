function Piece(color,type,position){
    this.color = color;
    this.type = type;
    this.position = position;
}

const pieceArray = Array.from({length : 8}, () => Array(8).fill(null));
const backRowsArray = ["Rook","Knight","Bishop","Queen","King","Bishop","Knight","Rook"];
let isWhiteTurn = true;
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
    let url = `http://localhost:8080/tile-clicked?row=${row}&col=${col}`
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
    let [originalRow,originalCol] = selectedPiece[0];
    if((pieceArray[originalRow][originalCol].color == "White" && !isWhiteTurn )||(pieceArray[originalRow][originalCol].color == "Black" && isWhiteTurn)){
        console.log(`currentTurn is ${pieceArray[originalRow][originalCol].color}, it is not your turn`);
        return;
    }

    let url = `http://localhost:8080/move-piece?pieces=`;
    
    url += createMovePieceString(originalRow,originalCol,row,col);
    console.log(url);
    const response = await fetch(url);
    let text = await response.text();

    if(text == "inValid"){
        console.warn("backend Error (check c++)");
        return;
    }

    //movement of pieces
    pieceArray[row][col] = pieceArray[originalRow][originalCol];
    pieceArray[row][col].position = [row, col];
    pieceArray[originalRow][originalCol] = null;

    //change board ui
    moveImage(originalRow,originalCol,row,col);

    if(pieceArray[row][col].type == "Pawn" && checkBackRank(pieceArray[row][col].color,row)){
        console.log(`pawn promotion detected at row: ${row} col: ${col}`);
        pawnPromotion(parseInt(row),parseInt(col),pieceArray[row][col].color);
    }

    isWhiteTurn = !isWhiteTurn;
}

function moveImage(originalRow,originalCol,row,col){
    let originalTile = document.querySelector(`[data-row="${originalRow}"][data-col="${originalCol}"]`);
    let newTile = document.querySelector(`[data-row="${row}"][data-col="${col}"]`);
    let capturedImg = newTile.querySelector('img');
    if(capturedImg) capturedImg.remove();
    newTile.appendChild(originalTile.querySelector('img'));
}

function checkBackRank(color,row){
    return color == "White" ? row == 7 : row == 0;
}

const availablePromotionPieces = ["q","r","b","n"]
const promotionPieces = [];
async function pawnPromotion(row,col,color){
    let dir = 0;
    let colorChar = colorToChar[color];
    while(Math.abs(dir) <= 3){
        console.log(`1m at pawnPromotion current row is ${row + dir}`);
        let promotionTile = document.querySelector(`[data-row="${row + dir}"][data-col="${col}"]`);
        //generate the css and html of the selected piece and store them into promotionPieces (used to deselect created divs once done/cancel)
        let promotionPiece = document.createElement("div");
        let promotionImage = document.createElement("img");
        let typeChar = availablePromotionPieces[Math.abs(dir)];
        promotionImage.src = generateImageFromType(colorChar, typeChar);
        promotionPiece.append(promotionImage);

        promotionPiece.classList.add("promotionOption");
        promotionPiece.dataset.promotionType = `${colorChar}${typeChar}`;
        promotionTile.append(promotionPiece);
        promotionTile.addEventListener('click',function(event){
            event.stopPropagation();
            promotionTileClicked(promotionPiece.dataset.promotionType, row, col)});

        promotionPieces.push(promotionPiece);
        color == "White" ? dir-- : dir++;
    }
}


async function promotionTileClicked(promotionType,promotedRow,promotedCol){
    const colorChar = promotionType[0];
    const typeChar = promotionType[1];

    let url = `http://localhost:8080/pawn-promotion?promoteValues=`;
    url += `${promotedRow}${promotedCol}${typeChar}${colorChar}`;

    const response = await fetch(url);
    const text = await response.text();
    if (text != "validated"){
        throw "promotion error, check backend (c++)";
    }

    clearPromotionPieces();

    const color = colorChar == 'w' ? 'White' : 'Black';
    const type = charToType[typeChar];

    let tile = document.querySelector(`[data-row="${promotedRow}"][data-col="${promotedCol}"]`);
    tile.querySelector('img').remove();

    pieceArray[promotedRow][promotedCol] = new Piece(color, type, [parseInt(promotedRow), parseInt(promotedCol)]);
    let newImg = document.createElement('img');
    newImg.src = generateImageFromType(colorChar, typeChar);
    tile.appendChild(newImg);
}

function clearPromotionPieces(){
    for(let i = 0; i < promotionPieces.length ; i++){
        promotionPieces[i].remove();
    }
}
function createMovePieceString(originalRow,originalCol,newRow,newCol){
    let pieceString = "";
    let originalPiece = pieceArray[originalRow][originalCol];
    if (originalPiece == null) {
        console.warn("error moving a piece that dosent exist")
        return;
    }
    pieceString += `${originalRow}${originalCol}${typeToChar[originalPiece.type]}${colorToChar[originalPiece.color]}`

    let newPiece = pieceArray[newRow][newCol];
    let isNewPiece = newPiece != null;

    pieceString += `${newRow}${newCol}${isNewPiece ? typeToChar[newPiece.type] : 'o'}${isNewPiece ? colorToChar[newPiece.color] : 'o'}`;
    
    return pieceString
}

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
            continue;
        }
        if(text[s] == "n"){
            continue;
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
    imageString += typeToChar[piece.type];
    imageString += ".png";
    return imageString;
}

//function assumesd it is already in char form
function generateImageFromType(color,type){
    return `static/images/vintage/${color}${type}.png`;
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


function placePiece(tile, color, type, row, col){
    pieceArray[row][col] = new Piece(color, type, [row, col]);
    let img = document.createElement('img');
    img.src = generateImageText(pieceArray[row][col]);
    tile.append(img);
}

const colorToChar = {
    'White' : 'w',
    'Black' : 'b'
}
const typeToChar = {
    'Pawn' : 'p',
    'Knight' : 'n',
    'Bishop' : 'b',
    'Rook' : 'r',
    'Queen' : 'q',
    'King' : 'k'
}

const charToType = {
    'q' : 'Queen',
    'r' : 'Rook',
    'b' : 'Bishop',
    'n' : 'Knight'
}

s