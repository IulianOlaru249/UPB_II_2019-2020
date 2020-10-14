{-# OPTIONS_GHC -Wall #-}
{-# LANGUAGE MultiParamTypeClasses #-}

module Search where

import ProblemState
{-
    *** TODO ***

    Tipul unei nod utilizat în procesul de căutare. Recomandăm reținerea unor
    informații legate de:

    * stare;
    * acțiunea care a condus la această stare;
    * nodul părinte, prin explorarea căruia a fost obținut nodul curent;
    * adâncime
    * copiii, ce vor desemna stările învecinate
-}

-- s e tipul de date State - adica (Level arr) 
-- a este tipul de date actiune - adica ((Int, Int), Directions)

-- Node (stare, actiune, parinte(stare_parinte, revers_actiune), depth, successors
data Node s a = Node s (Maybe a) (Maybe (Node s a)) Int [(Node s a)]
      deriving (Eq, 
      Show)
{-
    *** TODO ***
    Gettere folosite pentru accesul la câmpurile nodului
-}
nodeState :: Node s a -> s
nodeState (Node state _ _ _ _) = state

nodeParent :: Node s a -> Maybe (Node s a)
nodeParent (Node _ _ Nothing _ _) = Nothing
nodeParent (Node _ _ (Just parent) _ _) = Just parent

nodeDepth :: Node s a -> Int
nodeDepth (Node _ _  _ depth _) = depth

nodeAction :: Node s a -> Maybe a
nodeAction (Node _ Nothing _ _ _) = Nothing
nodeAction (Node _ (Just action) _ _ _) = Just action

nodeChildren :: Node s a -> [Node s a]
nodeChildren (Node _ _ _ _ children) = children

{-
    *** TODO ***

    Generarea întregului spațiu al stărilor
    Primește starea inițială și creează nodul corespunzător acestei stări,
    având drept copii nodurile succesorilor stării curente.
-}
createChildren :: (ProblemState s a, Eq s) => s -> a -> (Node s a) -> Int -> Node s a
createChildren state action parent depth = child
    where child = Node state (Just action) (Just parent) depth newChildren
          newChildren = [ createChildren (snd newSuc) (fst newSuc) child (depth + 1) | newSuc <- (successors state)]

createStateSpace :: (ProblemState s a, Eq s) => s -> Node s a
createStateSpace state = parent
    where parent = (Node state  Nothing Nothing 0 children)
          children = [createChildren (snd suc) (fst suc) parent 1 | suc <- (successors state)]

{-
    *** TODO ***
   
    Primește un nod inițial și întoarce un flux de perechi formate din:
    * lista nodurilor adăugate în frontieră la pasul curent
    * frontiera

-}
{-
dummyTree :: (ProblemState s a, Eq s) => s -> Node s a
dummyTree state = parent
    where parent = (Node state  Nothing Nothing 0 children)
          children = [(Node (snd (head (successors state))) (Just (fst (head (successors state)))) (Just parent) 1 [])]
-}

bfsHelper :: Ord s => Node s a -> [ ([Node s a], [Node s a]) ] -> [s] -> [([Node s a], [Node s a])]
bfsHelper cur_node acc visited
    --bfs se opreste cand nu mai avem ce adauga la frontiera, adica atunci cand nu mai exista noduri nevizitate
    | length aux == 0 = acc
    -- face apel recursiv pe primul nod din frontiera. Va retine nodurile care au putut fi adaugate frontierei iar nodul curent va fi eliminat din forntiera; 
    -- frontiera cu acele noduri iar la multimea de noduri vizitate se va adauga si acel nod
    | otherwise = bfsHelper top (acc ++ [(unvisitedChildren, dequeue ++ unvisitedChildren)]) (visited ++ [(nodeState cur_node)])
    where frontiera = snd (head acc)
          aux = fst (head acc)
          --frontiera se comporta ca o coada pentru BFS
          top = head frontiera
          dequeue = tail frontiera
          --din lista de copii (vecini) ai nodului ia-i doar pe aceia care nu sunt in visited
          unvisitedChildren = filter ( \child -> not (elem (nodeState child) visited) ) (nodeChildren cur_node)

bfs :: Ord s => Node s a -> [([Node s a], [Node s a])]
--primul nod din spatiu starilor va fi adaugat in frontiera si marcat ca vizitat
bfs node = bfsHelper node [([], [node])] [(nodeState node)]


{-
    *** TODO ***
  
    Primește starea inițială și finală și întoarce o pereche de noduri, reprezentând
    intersecția dintre cele două frontiere.
-}

bidirBFS :: Ord s => Node s a -> Node s a -> (Node s a, Node s a)
bidirBFS = undefined


{-
    *** TODO ***

    Pornind de la un nod, reface calea către nodul inițial, urmând legăturile
    către părinți.

    Întoarce o listă de perechi (acțiune, stare), care pornește de la starea inițială
    și se încheie în starea finală.

-}

extractPath :: Node s a -> [(Maybe a, s)]
extractPath = undefined



{-
    *** TODO ***

    Pornind de la o stare inițială și una finală, se folosește de bidirBFS pentru a găsi
    intersecția dintre cele două frontiere și de extractPath pentru a genera calea.

    Atenție: Pentru calea gasită în a doua parcurgere, trebuie să aveți grijă la a asocia
    corect fiecare stare cu acțiunea care a generat-o.

    Întoarce o listă de perechi (acțiune, stare), care pornește de la starea inițială
    și se încheie în starea finală.
-}

solve :: (ProblemState s a, Ord s)
      => s          -- Starea inițială de la care se pornește
      -> s          -- Starea finală la care se ajunge
      -> [(Maybe a, s)]   -- Lista perechilor
solve = undefined
