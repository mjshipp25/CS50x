SELECT title FROM movies 
WHERE id IN (SELECT movie_id FROM stars WHERE person_id IN (SELECT people.id FROM people WHERE people.name = "Johnny Depp"))
AND id IN (SELECT movie_id FROM stars WHERE person_id IN (SELECT people.id FROM people WHERE people.name = "Helena Bonham Carter"));