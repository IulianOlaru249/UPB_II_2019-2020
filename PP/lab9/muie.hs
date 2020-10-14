import Data.List
import Data.Char
import Data.Maybe

{- 
În acest exercițiu urmărim crearea și exploatarea unei tabele de frecvență pentru caracterele care apar în cuvinte diferite dintr-un text. O tabelă de frecvență este o colecție de asocieri între un caracter și frecvența sa de apariție. 
-}

{-
1. (10p)
Să se creeze aliasul de tip FreqTable, pentru tabele de frecvență.
Să se implementeze funcția rareChars, care primește o tabelă de frecvență și un număr n și întoarce lista caracterelor din tabelă care au frecvența mai mică decât n.

ex: 
*Main> rareChars [('a',3), ('b',1), ('c',2), ('d',1)] 2
"bd"
-}
--data Val c a = V [(c, a)]
type FreqTable = [(Char, Int)]

rareChars :: FreqTable -> Int -> [Char]
rareChars frq n = map (\(a, _) -> a) (filter (\x -> (snd x) < n) frq)
--rareChars frq n = map (\(a, _) -> a) filter (\x -> (snd x) < n) frq
--rareChars table n = [ V table]

{-
2. (10p)
Să se implementeze funcția insChar, care inserează un caracter într-o tabelă de frecvență (dacă el exista deja în tabelă, atunci frecvența este actualizată).

ex:
*Main> insChar 'e' [('a',3), ('b',1), ('c',2), ('d',1)]
[('e',1),('a',3),('b',1),('c',2),('d',1)]
*Main> insChar 'c' [('a',3), ('b',1), ('c',2), ('d',1)]
[('c',3),('a',3),('b',1),('d',1)]	-- s-a actualizat frecvența lui c
-}

insChar :: Char -> FreqTable -> FreqTable
insChar = undefined

{-
3. (10p)
Să se implementeze funcțiile textToStr și textToTable. 
textToStr primește un text și creează un string în care fiecare cuvânt din text apare o singură dată, și sunt eliminate spațiile albe.
textToTable primește un text și creează o tabelă de frecvență pentru cuvintele distincte din acesta (folosind rezultatul lui texToStr).

Utile: words :: String -> [String]	-- împarte un text în cuvinte, parsând după spații

ex:
*Main> textToStr "copacii albi copacii negri"
"copaciialbinegri"
*Main> textToTable "copacii albi copacii negri"
[('c',2),('o',1),('p',1),('a',2),('i',4),('l',1),('b',1),('n',1),('e',1),('g',1),('r',1)]
-}
textToStr :: String -> String
textToStr = undefined
										 
textToTable :: String -> FreqTable
textToTable = undefined