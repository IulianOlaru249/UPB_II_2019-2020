#lang racket

;; Exercitiul 1 - decomentati apelurile de mai jos pentru a 
;; observa comportamentul functiilor curry/uncurry
;; exercitiul a fost prezentat si la curs
(define add-curry
  (lambda (x)
    (lambda (y)
      (+ x y))))

;((add-curry 2) 5)       ;; 7 - observati apelul: intai (add-curry 2), apoi rezultatul aplicat pe 5
;(add-curry 2)           ;; #<procedure> - va fi o noua functie care asteapta un argument 
                        ;; pentru a il aduna cu 2

(define add-uncurry
  (lambda (x y)
    (+ x y)))

;(add-uncurry 2 5)       ;; 7
;(add-uncurry 2)         ;; procedure add-uncurry: expects 2 arguments, given 1: 2
                        ;; deci o eroare, intrucat o functie uncurry trebuie sa isi primeasca
                        ;; toate argumentele deodata

(define inc-curry (add-curry 1))
;(inc-curry 5)           ;; 6 - inc-curry este exact functia increment, definita aici cu
                        ;; minim de efort pe baza functiei add-curry
                        ;; ceea ce produce (add-curry 1) este (lambda (y) (+ x y)) cu x legat la 1

(define (inc-uncurry x) 
  (add-uncurry 1 x))
;(inc-uncurry 5)         ;; 6 - aceeasi functie increment, insa definita mai greoi (e nevoie de parametru)

;; Exercitiul 2 - mai multa reutilizare de cod, de data asta 
;; pentru a particulariza o functie de sortare
;; dupa orice tip de comparator
(define (ins-in-sorted op)
  (lambda (e L)
    (if (or (null? L) (op e (car L)))     ;; op = un operator de comparatie
        (cons e L)
        (cons (car L) ((ins-in-sorted op) e (cdr L))))))

(define (sort-ins op)
  (lambda (L)
    (if (null? L)
        L
        ((ins-in-sorted op) (car L) ((sort-ins op) (cdr L))))))

(define sort-ascending (sort-ins <=))
(define sort-descending (sort-ins >=))
(define sort-mod5 (sort-ins (lambda (x y) (< (modulo x 5) (modulo y 5)))))

;(sort-ascending '(6 1 7 9 3 2 4 6 7))      ;; o functie de sortare crescatoare
;(sort-descending '(6 1 7 9 3 2 4 6 7))     ;; o functie de sortare descrescatoare
;(sort-mod5 '(6 1 7 9 3 2 4 6 7))           ;; o functie de sortare dupa restul la impartirea cu 5

;; Observatie: sortarea mod 5 ar putea folosi o functie derivata dintr-o comparatie mod x, cu x oarecare
;; puteti incerca asta ca exercitiu

;; Exercitiul 3 - implementarea functionalei map
(define (map1 f L)
  (if (null? L)
      L
      (cons (f (car L)) (map1 f (cdr L)))))

;(map1 (lambda (x) (* 2 x)) '(1 2 3 4 5))
;(map1 (lambda (x) (even? x)) '(1 2 3 4 5))
;(map1 (lambda (x) (remainder x 3)) '(4 5 7 8 9 1))

;; Exercitiul 4 - implementarea functionalei filter
(define (filter1 p L)
  (cond
    ((null? L) L)
    ((p (car L)) (cons (car L) (filter1 p (cdr L))))
    (else (filter1 p (cdr L)))))

;(filter1 even? '(1 2 3 4 5 6 7 8))
;(filter1 positive? '(-3 -4 5 6 -8 2 -1))

;; Exercițiul 5 - implementarea funcționalei foldl
(define (foldl1 p acc L)
  (if (null? L)
      acc
      (foldl1 p (p (car L) acc) (cdr L))))

;(foldl1 + 0 '(1 2 3 4))
;(foldl1 (lambda (x acc) (if (>= x 10) (cons x acc) acc)) null '(1 3 10 11 4 12))
;(foldl1 cons null '(1 2 3 4))

;; Exercițiul 6 - implementarea funcționalei foldr
(define (foldr1 p acc L)
  (if (null? L)
      acc
      (p (car L) (foldr1 p acc (cdr L)))))

;(foldr1 + 0 '(1 2 3 4))
;(foldr1 cons null '(1 2 3 4))
;(foldr1 (lambda (x acc) (if (>= x 10) (cons x acc) acc)) null '(1 3 10 11 4 12))

;; Exercitiul 7 - suma elementelor unei liste utilizand apply
(define (sumlist L)
  (apply + L))

; (sumlist '(1 2 3 4 5))

;; Exercitiul 8 - suma elementelor unei liste utilizand foldr
(define (sumlist2 L)
  (foldr + 0 L))

; (sumlist2 '(1 2 3 4 5))

;; Exercitiul 9 - inversarea unei liste utilizand foldl
(define (reverselist L)
  (foldl cons null L))

; (reverselist '(1 2 3 4 5 6))

;; Exercițiul 10 BONUS - implementarea funcționalei apply
;; Pentru mai multe detalii legate de namespace și eval, accesati documentatia de Racket
;; (eval: Racket Documentation -> The Racket Guide -> Reflection and Dynamic Evaluation -> eval)
;; (namespaces: Racket Documentation -> The Racket Reference -> Language Model -> Syntax Model -> Namespaces)
(define (apply1 fun args)
  (eval (map (lambda (x) (list 'quote x)) (cons fun args)) (make-base-namespace)))


`2019-A
(define L1 (list 1 4 5 3))
(define L2 (list 1 3 2 4 1 5 3 9))
(map (λ(x) (cons x (length (filter (λ (y) (equal? x y)) L2)))) L1)

`2019-B
(define L3 (list (list 1 2 3) (list 1 2) (list 4 5) (list .5 .5)))
(filter (λ (x) (>= (apply + x) (apply * x)) ) L3)

`2019-C
(define Numbers (list 1 2 3 4 5 6 7 8 9 10))
(define (task p)
  ( λ (L) (filter (λ (x) (and (p x) (not (p (quotient x 2))))) L)))
((task even?) Numbers)

`2019-D
(define zip (lambda (l1 l2) (map list l1 l2)))
(define (task2 func L4 L5) (map (λ (x) (apply func x)) (zip L4 L5)))
(task2 + (list 1 2 3) (list 4 5 6))
(map (λ (x) (task2 + (list 1 2 3) x)) (list (list 4 5 6) (list 10 11 12)))

`2018-A
(define zip2 (lambda (l1 l2) (map list l1 l2)))


(zip2 (list 1 2 3) (list `a `b `c))

`2018-B
(define (unzip L) (cons (map car L) (map cdr L)))
(unzip (list (cons 1 `a) (cons 2 `b) (cons 3 `c)))

`2017-A
(define (task3 L)  (foldr max (foldr max 0 L) L))
(task3 (list 123 2 3))

`2017-B
(define (task4 L)  (foldr min (foldr max 0 L) L))
(task4 (list 123 2 3))

`2017-C
(define (task5 L) (filter (lambda (x) (null? (filter (lambda (y) (< (abs x) (abs y))) L))) L))
(task5 '(-1 23 2 -24))

`2016-A
(define (myAndMap L)(foldl (λ (a acc) (and a acc)) true L))
(myAndMap (list false true false))

`2016-B
(define (myOrMap L)(foldl (λ (a acc) (or a acc)) true L))
(myOrMap (list false true false))

`2015-A

`2015-B
(define (reunion L1 L2) (foldr (λ(x L)(if (member x L) L (cons x L))) `() (append L1 L2)))
(reunion '(1 2 3 4 7) '(3 4 5 6))

`2014-A
(define (task6 L1 L2) (filter (λ (x) (member (length x) L1)) L2))
(task6 `(1 2 3 4) `((4 5 6) () (a b)))

`2014-B
(define (task7 L1 L2) (filter (λ (x) (not (member (length x) L1))) L2))
(task7 `(1 0 4 5) `((4 5 6) () (a b)))

`2013-A
(define (task8 L2) (length (filter (λ (x) (odd? (car x))) L2)))
(task8 `((2 3 4) (1 2 3) (5 6) (8 9)))

(define sum
  (lambda (x y)
    (lambda ()
      (+ x y))))
(sum 1 2)

(let ((a 1) (b 2)) (let ((b 3) (c (+ b 2))) (+ a b c)))
(let* ((a 1) (b 2)) (let* ((b 3) (c (+ b 2))) (+ a b c)))
(let* ((a 1) (b 2) (c (+ a 2))) (+ a b c))
(let ((a 1)) (let ((b a)) (+ a b)))
(let* ((a 1) (b a)) (+ a b))
;(define a 2) (let ((a 1) (b a)) (+ a b))
(define a 2) (letrec ((a 1) (b a)) (+ a b))

(define (z x y)
  (if (or (null? x) (null? y))
      '()
      (cons (car x) (cons (car y) (z (cdr y) (cdr x))))))

;;(define (count-smaller L))
(define L10 (list 1 2 3 2 4 3 1 2))
(remove-duplicates (map (λ(x) (cons x (length (filter (λ (y) (> x y)) L10)))) L10))


(define s1 '((1 2) (3 4 5)))
(define s2 '((2 3) (1 4 5)))

(define my_zip (lambda (l1 l2) (map list l1 l2)))

(define (intersection a b)
  (if (null? a)
      '()
      (if (member (car a) b)
          (cons (car a) (intersection (cdr a) b))
          (intersection (cdr a) b))))

(intersection '(1 2) '(2 3))
(map my_zip s1 s2)
(apply intersection (map my_zip s1 s2))