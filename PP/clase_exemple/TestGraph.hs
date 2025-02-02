module TestGraph where

import Graph
import Data.List
import TestPP

{-
    Acesta este modulul de testare pentru acest laborator. Pentru a
    testa un anumit exercițiu rulați checkN, unde N este numărul
    execițiului. Mai jos aveti și runAllTests (sau check) care verifică
    toate exercițiile.

    Mai jos aveți graful pe care se face testarea:


    +++++              +++++
    + 1 +------------->+ 2 +
    +++++              +++++
      |  |\
      |   \
      |    -------
      |           \
      |            \
      \/            \
    +++++            \ +++++
    + 3 +------------->+ 4 +
    +++++              +++++
      |                  |
      |                  |
      |                  |
      \/                 \/
    +++++              +++++         +++++
    + 5 +------------->+ 6 +-------->+ 7 +
    +++++              +++++         +++++
      |                                /\
      |                                |
      |                                |
      ----------------------------------

-}

testNodes = [1..7]
testArcs = [(1, 2), (1, 3), (3, 4), (4, 1), (3, 5), (5, 6), (4, 6), (5, 7), (6, 7)]
sortedNodes = [1, 3, 4, 5, 6, 2, 7]

-- 1
test1 :: TestPP ()
test1 = testManually 1 1.5 -- 1.5p

-- 2
alGraph :: AdjListGraph
alGraph = build testNodes testArcs

test2 :: TestPP ()
test2 = testTemplate "[2] AdjListGraph " alGraph 2 -- 2p

-- 3
aGraph :: ArcGraph
aGraph = build testNodes testArcs

test3 :: TestPP ()
test3 = testTemplate "[3] ArcGraph " aGraph 1 -- 1p

-- 4
test4 :: TestPP ()
test4 = sequence_
  [ assertVal "[4] Conversion from AdjListGraph to ArcGraph works" 0.25 $ -- 0.25p
      convert alGraph `sameGraph` aGraph
  , assertVal "[4] Conversion from ArcGraph to AdjListGraph works" 0.25 $ -- 0.25p
      convert aGraph `sameGraph` alGraph
  ]

-- 5
test5 :: TestPP ()
test5 = sequence_
  [ assertVal "[5] DFS works as expected for AdjListGraph" 1.25 $ -- 1.25p
      dfs alGraph 1 `sameElements` expectedDFSTraversal
  , assertVal "[5] DFS works as expected for ArcGraph" 1.25 $ -- 1.25p
      dfs aGraph 1 `sameElements` expectedDFSTraversal
  ]
  where expectedDFSTraversal = [
          (1, Nothing), (2, Just 1), (3, Just 1),
          (4, Just 3), (6, Just 4), (7, Just 6), (5, Just 3)]

-- 6
test6 :: TestPP ()
test6 = sequence_
  [ assertVal "[6] BFS works as expected for AdjListGraph" 1.25 $ -- 1.25p
      bfs alGraph 1 `sameElements` expectedBFSTraversal
  , assertVal "[6] BFS works as expected for ArcGraph" 1.25 $ -- 1.25p
      bfs aGraph 1 `sameElements` expectedBFSTraversal
  ]
  where expectedBFSTraversal = [
          (1, Nothing), (2, Just 1), (3, Just 1),
          (4, Just 3), (5, Just 3), (6, Just 4), (7, Just 5)]

-- 7
alSGraph :: AdjListGraph
alSGraph = build sortedNodes testArcs

aSGraph :: ArcGraph
aSGraph = build sortedNodes testArcs

test7 :: TestPP ()
test7 = sequence_
  [ assertVal "[7] sortDegree works as expected for AdjListGraph" 0.25 $ -- 0.25p
      sortDegree alGraph `sameGraph` alSGraph
  , assertVal "[7] sortDegree works as expected for ArcGraph" 0.25 $ -- 0.25p
      sortDegree aGraph `sameGraph` aSGraph
  ]

-- 8
cGraph :: ColGraph
cGraph = build testNodes testArcs

test8 :: TestPP ()
test8 = sequence_
  [ testTemplate "[8] cGraph " cGraph 1 -- 1p
  , assertVal "[8] Conversion from ColGraph to ArcGraph works" 0.25 $ -- 0.25p
      convert cGraph `sameGraph` aGraph
  , assertVal "[8] Conversion from ColGraph to AdjListGraph works" 0.25 $ -- 0.25p
      convert cGraph `sameGraph` alGraph
  , assertVal "[8] ColGraph satisfies all constraints" 1 $ -- 1p
      let g = cGraph in and [(getColor g u) /= (getColor g v) | u <- (nodes g), v <- (nodes g), arcExists g u v]
  ]

-- 9, 10
test9, test10 :: TestPP ()
test9 = testManually 9 1 -- 1p
test10 = testManually 10 2 -- 1p

-- All
allTests = [test1, test2, test3, test4, test5, test6, test7 ,test8, test9, test10]
runAllTests = runTestPP $ sequence_ allTests
check = runAllTests
-- teste individuale 
[check1, check2, check3, check4, check5, check6, check7, check8, check9, check10] =
  map runTestPP allTests

-- Utilities
testTemplate :: Graph g => String -> g -> Float -> TestPP ()
testTemplate typeName g grade = sequence_
  [ assertVal (typeName ++ "gives the correct set of nodes") (grade * 0.2) $
      nodes g `sameElements` testNodes
  , assertVal (typeName ++ "gives the correct set of arcs") (grade * 0.2) $
      arcs g `sameElements` testArcs
  , assertVal (typeName ++ "implements nodeOut correctly") (grade * 0.2) $
      nodeOut g 1 `sameElements` [3,2] &&
      nodeOut g 2 `sameElements` [] &&
      nodeOut g 3 `sameElements` [4,5] &&
      nodeOut g 5 `sameElements` [6,7]
  , assertVal (typeName ++ "implements nodeIn correctly") (grade * 0.2) $
      nodeIn g 6 `sameElements` [4,5] &&
      nodeIn g 2 `sameElements` [1] &&
      nodeIn g 1 `sameElements` [4]
  , assertVal (typeName ++ "implements arcExists correctly") (grade * 0.2) $
      and [arcExists g a b | (a, b) <- testArcs ]
  ]

testManually :: Int -> Float -> TestPP ()
testManually testnum grade =
  assertVal ("[" ++ show testnum ++ "] TODO -- check manually") grade False

sameElements :: (Eq a) => [a] -> [a] -> Bool
sameElements xs ys = null (xs \\ ys) && null (ys \\ xs)

sameGraph :: Graph g => g -> g -> Bool
sameGraph g1 g2 = nodes g1 `sameElements` nodes g2 && arcs g1 `sameElements` arcs g2
