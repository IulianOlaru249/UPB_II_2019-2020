{-# OPTIONS_GHC -Wall #-}
{-# LANGUAGE EmptyDataDecls, MultiParamTypeClasses,
             TypeSynonymInstances, FlexibleInstances,
             InstanceSigs #-}
module RollTheBall where
import Pipes
import ProblemState
--import qualified Data.Array as A
import Data.Array as A

{-
    Get cell at position pos in array
-}
cellAt :: (A.Array (Int, Int) Cell) -> (Int, Int) -> Cell
cellAt arr pos = arr A.! pos

{- 
    Lower right corner of array 
-}
lowerRightCorner :: (A.Array (Int, Int) Cell) -> (Int, Int)
lowerRightCorner = snd . A.bounds

elemsList :: (A.Array (Int, Int) Cell) -> [Cell]
elemsList arr = A.elems arr

{-
    Direcțiile în care se poate mișca o piesa pe tablă
-}
data Directions = North | South | West | East
    deriving (Show, Eq, Ord)

{-
    Sinonim tip de date pentru reprezetarea unei perechi (Int, Int)
    care va reține coordonatele celulelor de pe tabla de joc
-}
type Position = (Int, Int)

{-
    Tip de date pentru reprezentarea celulelor tablei de joc
-}
-- Cell va avea incapsulat un pipe pe care il va printa
data Cell = Cell Char
        deriving (Eq, Ord)

-- pentru a preveni printari de caracter intre ghilimele se pune [val]
-- E.g.: a in loc de 'a'
instance Show Cell
    where show (Cell el) = [el]

{-
    Tip de date pentru reprezentarea nivelului curent
-}
data Level = Level (A.Array (Int, Int) Cell)
    deriving (Eq, Ord)

getCellVal :: Cell -> Char
getCellVal (Cell val) = val

--foldl pe functia A.assoc; helper cu acc \n cand j == 0. \n si la final ultimul \n 
printCell :: [Char] -> ((Int, Int) , Cell) -> [Char]
printCell acc val = if (snd (fst val)) == 0 then acc ++ [endl] ++ show  (snd val)
                    else acc ++ show (snd val)

{-
    *** Optional *** 
  
    Dacă aveți nevoie de o funcționalitate particulară,
    instantiați explicit clasele Eq și Ord pentru Level.
    În cazul acesta, eliminați deriving (Eq, Ord) din Level.
-}

{-
    Instanțiati Level pe Show.
    Atenție! Fiecare linie este urmată de \n (endl in Pipes).
-}
instance Show Level
    where show (Level state) = foldl  (printCell) [] (A.assocs state) ++ [endl]

{-
    Primește coordonatele colțului din dreapta jos a hărții.
    Intoarce un obiect de tip Level în care tabla este populată
    cu EmptySpace. Implicit, colțul din stânga sus este (0,0)
-}
emptyLevel :: Position -> Level
emptyLevel (m, n) = Level (A.array boundries values)
                    where boundries = ((0, 0), (m, n)) --dimensiunile
                          values =  [((x, y), el) | x <- [0..m], y <- [0..n]] --valorile
                          el = (Cell emptySpace)

{-
    *** TODO ***

    Adaugă o celulă de tip Pipe în nivelul curent.
    Parametrul char descrie tipul de tile adăugat: 
        verPipe -> pipe vertical
        horPipe -> pipe orizontal
        topLeft, botLeft, topRight, botRight -> pipe de tip colt
        startUp, startDown, startLeft, startRight -> pipe de tip initial
        winUp, winDown, winLeft, winRight -> pipe de tip final
    Parametrul Position reprezintă poziția de pe hartă la care va fi adaugată
    celula, dacă aceasta este liberă (emptySpace).
-}

addCell :: (Char, Position) -> Level -> Level
addCell (pipe, pos) (Level state)
    --verifica index out of bounds
    | ((fst pos) < 0) || ((snd pos) < 0) = (Level state)
    | ((fst pos) > cornerX) || ((snd pos) > cornerY) = (Level state)
    --o celula poate fi adaugata doar peste un spatiu gol
    | (getCellVal (cellAt state pos)) == (getCellVal (Cell emptySpace)) = Level (state A.//[(pos, (Cell pipe))])
    where cornerX = fst (lowerRightCorner state)
          cornerY = snd (lowerRightCorner state)

addCell (_, _) (Level _) = emptyLevel (0, 0)

{-
    *** TODO *** 

    Primește coordonatele colțului din dreapta jos al hărții și o listă de 
    perechi de tipul (caracter_celulă, poziția_celulei).
    Întoarce un obiect de tip Level cu toate celeule din listă agăugate pe
    hartă.
    Observatie: Lista primită ca parametru trebuie parcursă de la dreapta 
    la stanga.
-}
createLevel :: Position -> [(Char, Position)] -> Level
createLevel corner pipes = foldr addCell (emptyLevel corner) pipes

{-
    *** TODO ***

    Mișcarea unei celule în una din cele 4 direcții
    Schimbul se poate face doar dacă celula vecină e goală (emptySpace).
    Celulele de tip start și win sunt imutabile.

    Hint: Dacă nu se poate face mutarea puteți lăsa nivelul neschimbat.
-}
getNextPos :: Position -> Directions -> Position
getNextPos (x, y) North = (x - 1, y)
getNextPos (x, y) South = (x + 1, y)
getNextPos (x, y) West  = (x, y - 1)
getNextPos (x, y) East  = (x, y + 1)

--plasare peste ceva diferit de empty cell
--verifica coordonate invalide;
--index out of bounds
--daca e immutable
--cazuul bun: pune empty cell unde era asta

moveCell :: Position -> Directions -> Level -> Level
moveCell initialPos dir (Level state)
    -- daca urmatoarea pozitie are index out of bounds
    | ((fst nextPos) < 0) || ((snd nextPos) < 0) = Level state
    | ((fst nextPos) > (fst corner)) || ((snd nextPos) > (snd corner)) = Level state
    -- daca incercam sa mutam oricare tip de pipe de start / win
    | isStartPipe = Level state
    | isWinPipe = Level state
    -- daca incercam sa mutam pe un spatiu deja ocupat
    | (getCellVal (cellAt state nextPos)) /= (getCellVal (Cell emptySpace)) = Level state
    -- daca mutarea se poate efectua
    | (getCellVal (cellAt state nextPos)) == (getCellVal (Cell emptySpace)) = Level (state A.//[
                                                                        (initialPos, (Cell emptySpace)),
                                                                        (nextPos, (cellAt state initialPos))
                                                                    ])
    where nextPos = getNextPos initialPos dir
          corner = lowerRightCorner state
          isStartPipe = length (filter (== (cellAt state initialPos)) [(Cell startUp), (Cell startDown), (Cell startLeft), (Cell startRight)]) /= 0
          isWinPipe = length (filter (== (cellAt state initialPos)) [(Cell winUp), (Cell winDown), (Cell winLeft), (Cell winRight)]) /= 0

moveCell _ _ (Level _) = emptyLevel (0, 0)

{-
    *** HELPER ***

    CEA mai jegoasa functie POSIBILA. Gandit 5 min scris 45 min
    pf..
    Verifică dacă două celule se pot conecta.
    Atenție: Direcția indică ce vecin este a
    doua celulă pentru prima.

    ex: connection botLeft horPipe East = True (╚═)
        connection horPipe botLeft East = False (═╚)
-}
connection :: Cell -> Cell -> Directions -> Bool
--prima teava poate avea in dreapta/stanga/sus/jos doar tevile din lista. Daca a doua teava nu e printre ele, atunci nu se pot conecta

connection (Cell first) (Cell second) East
    | first == horPipe = length (filter (== second) [horPipe, topRight, botRight, winLeft]) /= 0
    | first == topLeft = length (filter (== second) [horPipe, botRight, topRight, winLeft]) /= 0
    | first == botLeft = length (filter (== second) [horPipe, botRight, topRight, winLeft]) /= 0
    | first == startRight = length (filter (== second) [horPipe, botRight, topRight, winLeft]) /= 0

connection (Cell first) (Cell second) West
    | first == horPipe = length (filter (== second) [horPipe, topLeft, botLeft, winRight]) /= 0
    | first == topRight = length (filter (== second) [horPipe, botLeft, topLeft, winRight]) /= 0
    | first == botRight = length (filter (== second) [horPipe, botLeft, topLeft, winRight]) /= 0
    | first == startLeft = length (filter (== second) [horPipe, topLeft, botLeft, winRight]) /= 0

connection (Cell first) (Cell second) North
    | first == verPipe = length (filter (== second) [verPipe, topLeft, topRight, winDown]) /= 0
    | first == botLeft = length (filter (== second) [verPipe, topLeft, topRight, winDown]) /= 0
    | first == botRight = length (filter (== second) [verPipe, topLeft, topRight, winDown]) /= 0
    | first == startUp = length (filter (== second) [verPipe, topLeft, topRight, winDown]) /= 0

connection (Cell first) (Cell second) South
    | first == verPipe = length (filter (== second) [verPipe, botLeft, botRight, winUp]) /= 0
    | first == topLeft = length (filter (== second) [verPipe, botLeft, botRight, winUp]) /= 0
    | first == topRight = length (filter (== second) [verPipe, botRight, botLeft, winUp]) /= 0
    | first == startDown = length (filter (== second) [verPipe, botLeft, botRight, winUp]) /= 0

connection (Cell _) (Cell _) _ = False

{-
    *** TODO ***

    Va returna True dacă jocul este câștigat, False dacă nu.
    Va verifica dacă celulele cu Pipe formează o cale continuă de la celula
    de tip inițial la cea de tip final.
    Este folosită în cadrul Interactive.
-}
findStartCellPos :: Level -> Position
findStartCellPos (Level state)
    --nu exista nivel fara celula de start
    | (null down) /= True = fst $ head down
    | (null up) /= True = fst $ head up
    | (null right) /= True = fst $ head right
    | (null left) /= True = fst $ head left
    where down = filter ((==(Cell startDown)).snd) cells
          up = filter ((==(Cell startUp)).snd) cells
          right = filter ((==(Cell startRight)).snd) cells
          left = filter ((==(Cell startLeft)).snd) cells
          cells = [cell | cell <- A.assocs state]

findStartCellPos (Level _) = (-1, -1)

outOfBounds :: Position -> Level -> Bool
outOfBounds pos (Level state)
    | ((fst pos) < 0) || ((snd pos) < 0) = True
    | ((fst pos) > (fst corner)) || ((snd pos) > (snd corner)) = True
    | otherwise = False
    where corner = lowerRightCorner state

wonLevelHelper :: Position -> Level -> Bool
wonLevelHelper pos (Level state)
    -- daca celula e win true
    | length (filter (== head (show (cellAt state pos))) winningCells) /= 0 = True
    -- daca e conexiune nord apel recursiv pe pos Nord
    | if (outOfBounds northPos (Level state)) then False else connection (cellAt state pos) (cellAt state northPos) North = wonLevelHelper northPos (Level (state A.// [(pos, (Cell emptySpace))]))
    -- daca e conexiune sud apel recursiv pe pos Sud
    | if (outOfBounds southPos (Level state)) then False else connection (cellAt state pos) (cellAt state southPos) South = wonLevelHelper southPos (Level (state A.// [(pos, (Cell emptySpace))]))
    -- daca e conexiune est apel recursiv pe pos Est
    | if (outOfBounds eastPos (Level state)) then False else connection (cellAt state pos) (cellAt state eastPos) East = wonLevelHelper eastPos (Level (state A.// [(pos, (Cell emptySpace))]))
    -- daca e conexiune vest apel recursiv pe pos Vest
    | if (outOfBounds westPos (Level state)) then False else connection (cellAt state pos) (cellAt state westPos) West = wonLevelHelper westPos (Level (state A.// [(pos, (Cell emptySpace))]))
    -- daca nu e conexiune, fals
    | otherwise = False
    where northPos = getNextPos pos North
          southPos = getNextPos pos South
          eastPos = getNextPos pos East
          westPos = getNextPos pos West

wonLevel :: Level -> Bool
wonLevel (Level state) = wonLevelHelper startPos (Level state)
    where startPos = findStartCellPos (Level state)

instance ProblemState Level (Position, Directions) where
    -- primeste un nivel si intoarce starile imediat urmatoare
    successors (Level state) = [suc | suc <- allValidStates]
               -- sunt valide doar acele configuratii care difera de cea initiala, adica cele pentru care s-a
               -- putut efectua mutarea
        where allValidStates = filter ((/= (Level state)).snd) allStates 
               -- se obtine o lista a configuratiilor in urma efectuarii mutarilor in fiecare directie pe fiecare
               -- dintre celulele configuratiei initiale
              allStates = [ ((celPos, dir), (moveCell celPos dir (Level state))) | dir <- directions, celPos <- allCellPositions]
              directions = [North, South, East, West]
              allCellPositions = [fst cell | cell <- A.assocs state]
    
    -- starea curenta castiga jocul
    isGoal level = wonLevel level

    -- inverseaza parametrii si pozitia
    reverseAction ((pos, North), level) = (((getNextPos pos North), South), (moveCell pos South level))
    reverseAction ((pos, South), level) = (((getNextPos pos South), North), (moveCell pos North level))
    reverseAction ((pos, East), level) = (((getNextPos pos East), West), (moveCell pos West level))
    reverseAction ((pos, West), level) = (((getNextPos pos West), East), (moveCell pos East level))