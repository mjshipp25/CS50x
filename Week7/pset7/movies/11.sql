SELECT movies.title FROM movies
JOIN ratings ON movies.id = ratings.movie_id
JOIN stars ON stars.movie_id = movies.id
WHERE person_id IN(SELECT people.id FROM people WHERE name = "Chadwick Boseman")
ORDER BY rating DESC LIMIT 5;