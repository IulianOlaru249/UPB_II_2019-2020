{-
  PP, laboratorul 8: tipuri de date utilizator
-}

import Data.List
import Data.Maybe
import TestPP

type Value = Int
data Node2 = Node2 Value

f x = [f $ x + 1]


-- \x y -> x + 1 / 2 $ 5 2
-- f = (+ 2) $ 1

-- data D = Pair Int Int
-- type D = Int Int
-- data D = D a b
-- data D a = D a a

--f x y z = if x
--  then if (length $ head y) == (head $ tail z) then "Success" else "Failure" else ""

--take 5 $ zipWith (+) (iterate (+1) 0)
--  (iterate (+2) 0)

{-
  1. (2p) Vectori
  Se dă tipul de date Vector, reprezentând vectori din spațiul R^3.

  Implementați următoarele operații cu vectori:
  - norma unui vector
  - normalizarea unui vector
  - produsul scalar (dot product) dintre doi vectori
  - produsul vectorial (cross product) dintre doi vectori
  - adunarea a doi vectori
  - scăderea a doi vectori
  - verificarea ortogonalității unor vectori

  Explicații

  Fie a și b doi vectori din R^3 considerați de forma:
  a = a1 * i + a2 * j + a3 * k
  b = b1 * i + b2 * j + b3 * k
  Norma vectorului a este egală cu:
  |a| = sqrt(a1^2 + a2^2 + a3^2)
  Normalizarea vectorului a este egală cu:
  a' = a / |a| = (a1 / |a|) * i + (a2 / |a|) * j + (a3 / |a|) * k
  Produsul vectorial al celor doi vectori o să fie egal cu:
  a x b = (a2 * b3 - a3 * b2) * i + (a3 * b1 - a1 * b3) * j + (a1 * b2 - a2 * b1) * k
  Produsul scalar al celor doi vectori o să fie egal cu:
  a • b = a1 * b1 + a2 * b2 + a3 * b3
  Produsul scalar a doi vectori u și v este 0 dacă și numai dacă u și v sunt ortogonali.

  Pentru mai multe detalii, consultați:
  https://gerardnico.com/linear_algebra/vector_vector
-}
data Vector = V
  { vx :: Double
  , vy :: Double
  , vz :: Double
  } deriving (Show, Eq)

lengthV :: Vector -> Double
lengthV (V vx vy vz) = sqrt $ vx * vx + vy * vy + vz * vz

normalizeV :: Vector -> Vector
normalizeV (V vx vy vz) = V (vx / (lengthV (V vx vy vz)))
                            (vy / (lengthV (V vx vy vz)))
                            (vz / (lengthV (V vx vy vz)))


dotV :: Vector -> Vector -> Double
dotV (V a1 a2 a3) (V b1 b2 b3) = a1 * b1 + a2 * b2 + a3 * b3

crossV :: Vector -> Vector -> Vector
crossV (V a1 a2 a3) (V b1 b2 b3) = V ((a2 * b3) - (a3 * b2))
                                     ((a3 * b1) - (a1 * b3))
                                     ((a1 * b2) - (a2 * b1))

addV :: Vector -> Vector -> Vector
addV (V a1 a2 a3) (V b1 b2 b3) = V (a1 + b1) (a2 + b2) (a3 + b3)

subV :: Vector -> Vector -> Vector
subV (V a1 a2 a3) (V b1 b2 b3) = V (a1 - b1) (a2 - b2) (a3 - b3)

orthogonalV :: [Vector] -> Bool
orthogonalV v_list = if (1 == length v_list)
                     then True
                     else if (0 /= dotV (head v_list) (head $ tail v_list))
                            then False
                            else orthogonalV (tail v_list)

check1 :: TestData
check1 = do
  let v1 = V 1 (-1) 0
      v2 = V 1 1 0
      v3 = V 0 0 0
  tests 1 2 $
          [ testVal "lengthV" 1 (lengthV v1) (sqrt 2)
          , testVal "normalizeV" 1 (normalizeV (V 0 (-1) 0)) (V 0 (-1) 0)
          , testVal "dotV" 1 (dotV v1 v2) 0.0
          , testVal "crossV" 1 (crossV v1 v2) (V 0 0 2)
          , testVal "addV" 1 (addV v1 v2) (V 2 0 0)
          , testVal "subV" 1 (subV v2 v3) v2
          , testCond "orthogonalV" 2 (orthogonalV [v1, v2, v3])]

{-
 2. (4p) Liste imbricate
  Definiți un tip de date SList a care să aibă funcționalități
  asemănătoare listelor din limbajele Lisp (e.g. Scheme, Racket, Clojure),
  permițând componente la diferite niveluri de imbricare.
  Ex: Lista din Racket '(1 (3 4) (2)) să poată fi definită în Haskell
  folosind SList.
  Adițional, definiți:
  - emptySList, lista vidă
  - consElem, adaugă un element în capul unei liste
    Ex: consElem 1 '((3 4) (2)) == '(1 (3 4) (2))
  - consList, adaugă o listă (imbricată) în capul unei liste
    Ex: consList '(2 3) '(1 2) == '((2 3) 1 2)
  - headSList, ia primul element dintr-un SList
  - tailSList, ia restul SList-ului
  - deepEqual, o funcție ce verifică egalitatea a două SList
  - flatten, întoarce lista cu elementele din SList (pe același nivel)
  Notare:
  (2p) constructorii (emptySList, consElem și consList) și deepEqual
  (1p) headSList și tailSList
  (1p) flatten
-}
data SList a = EmptyList |
               ConsElem a (SList a) |
               ConsList (SList a) (SList a) deriving Show

emptySList :: SList a
emptySList = EmptyList

consElem :: a -> SList a -> SList a
consElem a s_list = ConsElem a s_list

consList :: SList a -> SList a -> SList a
consList s_list1 s_list2 = ConsList s_list1 s_list2

headSList :: SList a -> SList a
headSList EmptyList = EmptyList
headSList (ConsElem a s_list) = ConsElem a EmptyList
headSList (ConsList s_list1 s_list2) = s_list1

tailSList :: SList a -> SList a
tailSList EmptyList = EmptyList
tailSList (ConsElem a s_list) = s_list
tailSList (ConsList s_list1 s_list2) = s_list2

deepEqual :: Eq a => SList a -> SList a -> Bool
deepEqual EmptyList EmptyList = True
deepEqual s_list EmptyList = False
deepEqual EmptyList s_list = False
deepEqual (ConsElem a1 s_list1) (ConsElem a2 s_list2) = if a1 /= a2
                                                then False
                                                else deepEqual s_list1 s_list2
deepEqual s_list1 s_list2 = if (deepEqual (headSList s_list1) (headSList s_list2)) == False
                        then False
                        else deepEqual (tailSList s_list1) (tailSList s_list1)

flatten :: SList a -> [a]
flatten EmptyList = []
flatten (ConsElem a s_list) = a : flatten s_list
flatten (ConsList s_list1 s_list2) = (flatten s_list1) ++ (flatten s_list2)


check2 :: TestData
check2 = do
  let l1 = consElem 1 $ emptySList
      l2 = consElem 2 $ consList (consElem 1 $ consElem 1 emptySList) $
           consElem 3 emptySList
      l3 = consList (consElem 1 $ consElem 1 emptySList) $ consElem 3 $
           emptySList
  tests 2 4 $
          [ testCond "simple lists" 1 $ deepEqual l1 l1 && not (deepEqual l1 l2)
          , testCond "less simple lists" 1 $ deepEqual (consElem 2 $ l3) l2
          , testCond "head, tail" 1 $ deepEqual (headSList $ tailSList l2)
                                   (consElem 1 $ consElem 1 emptySList)
          , testCond "flatten" 1 $ flatten l2 == [2,1,1,3]]

{-
  3. (4p) Arbori binari de căutare
  Definiți un tip de date BST a pentru a implementa un arbore binar de
  căutare. De asemenea, definiți funcții pentru a insera o valoare într-un 
  arbore binar de căutare, căutarea unui element într-un arbore binar de 
  căutare dat, o funcție care întoarce lista elementelor din parcurgerea
  în inordine a arborelui și o funcție care întoarce cel mai mic subarbore
  care conține două noduri ale căror chei sunt date.
-}
data BST a = NewNode a (BST a) (BST a) |
             BSTNil deriving Show

insertElem :: (Ord a, Eq a) => BST a -> a -> BST a
insertElem BSTNil a = NewNode a BSTNil BSTNil
insertElem (NewNode root left_child right_child) a = if (a <= root)
                         then (NewNode root (insertElem left_child a) right_child)
                         else (NewNode root left_child (insertElem right_child a))

findElem :: (Ord a, Eq a) => BST a -> a -> Maybe a
findElem BSTNil a = Nothing
findElem (NewNode root left_child right_child) a = if a == root then Just a else
                if a < root then findElem left_child a
                             else findElem right_child a

subTree :: (Ord a, Eq a) => BST a -> a -> a -> Maybe (BST a)
subTree BSTNil a1 a2 = Nothing
subTree (NewNode root left_child right_child) a1 a2 = if ((root == a1) && (root == a2))
                                then Just (NewNode root left_child right_child)
                                else if (((root >= a1) && (root < a2)) || ((root <= a1) && (root > a2)))
                                     then Just (NewNode root left_child right_child)
                                     else if ((root > a1) && (root > a2))
                                          then subTree left_child a1 a2
                                          else subTree right_child a1 a2


inorder :: BST a -> [a]
inorder BSTNil = []
inorder (NewNode root left_child right_child) = (inorder left_child) ++ [root] 
                                                ++ (inorder right_child)

check3 :: TestData
check3 = do
  let root = foldl insertElem BSTNil [7, 4, 12, 2, 3, 1, 10, 15, 8]
      values = [1, 2, 3, 4, 7, 8, 10, 12, 15]
      tree1 = subTree root 1 3 
      tree2 = subTree root 4 5
      tree3 = subTree root 7 15
      tree4 = subTree root 1 8
  tests 3 4 $ 
          [ testVal "findElem" 0.5 (findElem root 3) (Just 3)
          , testVal "findElem" 0.5 (findElem root 5) Nothing
          , testSet "inorder" 1 (inorder root) values
          , testSet "subTree" 0.5 (inorder (fromJust tree1)) [1, 2, 3]
          , testCond "subTree" 0.5 (isNothing tree2)
          , testSet "subTree" 0.5 (inorder (fromJust tree3)) values
          , testSet "subTree" 0.5 (inorder (fromJust tree4)) values]

{-
  4. (BONUS, 2p) Arbore multicăi nevid
  Având dat tipul Tree a, definiți funcționala analoagă lui map, care să
  aplice o funcție asupra cheilor nodurilor din arbore, și o funcțională
  analoagă lui foldl care să parcurgă nodurile în ordinea: rădăcină, copil_1,
  copil_2, ... copil_n. 
-}

data Tree a = Node
  { value :: a
  , children :: [Tree a]
  } deriving (Eq, Show)

mapTree :: (a -> b) -> Tree a -> Tree b
mapTree = undefined

foldlTree :: (b -> a -> b) -> b -> Tree a -> b
foldlTree = undefined

check4 :: TestData
check4 = do
  let tree = Node 1 [Node 2 [Node 3 []], Node 4 [Node 5 [], Node 6 [Node 7 [], Node 8 []]]]
  tests 4 2 $
          [ testSet "foldlTree" 0.5 (foldlTree (flip (:)) [] tree) [1..8]
          , testVal "foldlTree" 0.5 (foldlTree (+) 0 tree) 36
          , testVal "mapTree" 1 (foldlTree (+) 0 $ mapTree (*2) tree) 72]

{-
  5. (BONUS, 3p) Difference lists
  Se cere definirea tipului de date "difference list".

  Un difference list este o listă "parțial construită", i.e. ale cărei
  elemente din coadă nu sunt (neapărat) în întregime cunoscute. De
  exemplu, ne putem imagina existența unei liste:

  1 : (2 : (3 : xs)) = [1,2,3] ++ xs

  unde xs nu are (la momentul construirii) o valoare cunoscută.

  În limbajele funcționale putem modela difference lists folosindu-ne de
  închideri: putem privi o astfel de structură ca pe o funcție care
  așteaptă un parametru (o listă) și întoarce o listă. Exemplul anterior
  poate fi astfel exprimat în funcție drept următoarea listă:

  (\ xs -> [1,2,3] ++ xs)

  Observație: Care este tipul lambda-ului de mai sus?

  Avantajul acestei abordări este că permite efectuarea oricărei
  operație de adăugare în listă (e.g. concatenarea cu o altă listă) în
  O(1), cu dezavantajul că eliminarea este în general mai puțin eficientă,
  deoarece presupune evaluarea elementelor structurii.

  Se cere, mai concret:
  - Definirea ADT-ului difference list (DList), „împăturit peste” o
    funcție de tipul [a] -> [a] (e.g. folosind construcția newtype)
  - Conversia [a] -> DL a (dlFromList) și invers (dlToList)
  - Lista vidă (emptyDL), adăugarea în capul unei liste (consDL) și în
    coada ei (snocDL)
  - Concatenarea a două liste (appendDL)
  - Operații de eliminare: primul element (headDL) și coada (tailDL)
    unei liste

  Operațiile de lucru cu difference lists (cu excepția celor de
  eliminare) vor fi implementate cât mai eficient posibil, i.e. fără a
  folosi dlFromList și dlToList.

  Pentru mai multe detalii, consultați link-ul:
  https://wiki.haskell.org/Difference_list
-}
newtype DList a = DL a -- de rafinat tipul argumentului lui DL

dlFromList :: [a] -> DList a
dlFromList = undefined

dlToList :: DList a -> [a]
dlToList = undefined

emptyDL :: DList a
emptyDL = undefined

consDL :: a -> DList a -> DList a
consDL = undefined

snocDL :: a -> DList a -> DList a
snocDL = undefined

appendDL :: DList a -> DList a -> DList a
appendDL = undefined

headDL :: DList a -> Maybe a
headDL = undefined

tailDL :: DList a -> Maybe (DList a)
tailDL = undefined

check5 :: TestData
check5 = do
  tests 5 3 $
          [ testCond "toList, fromList" 1 $ dlToList (dlFromList "Ana are mere") == "Ana are mere"
          , testCond "cons, empty" 1 $ dlToList (consDL 1 $ consDL 2 $ consDL 3 emptyDL) == [1,2,3]
          , testCond "snoc, empty" 1 $ dlToList (snocDL 1 $ snocDL 2 $ snocDL 3 emptyDL) == [3,2,1]
          , testCond "append" 1 $ dlToList (dlFromList [1,2,3] `appendDL` dlFromList [4,5,6]) ==
                                            [1,2,3,4,5,6]
          , testCond "head, tail" 2 $ case tailDL (dlFromList [1,2,3,4,5]) of
                                      Just dl -> case tailDL dl of
                                                 Just dl -> headDL dl == Just 3]

{-
Helpers for testing :) You can also run check1, check2 etc independently
-}
check = quickCheck [check1, check2, check3, check4, check5]
