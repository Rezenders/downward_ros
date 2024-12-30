(define (domain plansys2)
(:requirements :strips :typing :adl :negative-preconditions)

;; Types ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(:types
person
message
robot
room
);; end Types ;;;;;;;;;;;;;;;;;;;;;;;;;

;; Predicates ;;;;;;;;;;;;;;;;;;;;;;;;;
(:predicates

(robot_talk ?r - robot ?m - message ?p - person)
(robot_near_person ?r - robot ?p - person)
(robot_at ?r - robot ?ro - room)
(person_at ?p - person ?ro - room)

);; end Predicates ;;;;;;;;;;;;;;;;;;;;
;; Functions ;;;;;;;;;;;;;;;;;;;;;;;;;
(:functions

);; end Functions ;;;;;;;;;;;;;;;;;;;;
;; Actions ;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(:action move
    :parameters (?r - robot ?r1 ?r2 - room)
    :precondition (and
        (robot_at ?r ?r1)
        (not(robot_at ?r ?r2)))
    :effect (and
        (not(robot_at ?r ?r1))
        (robot_at ?r ?r2)
    )
)

(:action talk
    :parameters (?r - robot ?p - person ?m - message)
    :precondition (and
        (robot_near_person ?r ?p)
    )
    :effect (and
        (robot_talk ?r ?m ?p)
    )
)

(:action approach
    :parameters (?r - robot ?ro - room ?p - person)
    :precondition (and
        (robot_at ?r ?ro)
        (person_at ?p ?ro)
    )
    :effect (and
        (robot_near_person ?r ?p)
    )
)



);; end Domain ;;;;;;;;;;;;;;;;;;;;;;;;
