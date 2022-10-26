WITH cruise_movies AS (
     SELECT
          crew.title_id AS title_id
     FROM crew
     INNER JOIN
          people ON crew.person_id = people.person_id
     WHERE people.name LIKE "%Cruise%" AND people.born = 1962
)
SELECT
     titles.primary_title as name,
     ratings.votes as votes
FROM
     cruise_movies
INNER JOIN
     ratings ON cruise_movies.title_id = ratings.title_id
INNER JOIN
     titles ON cruise_movies.title_id = titles.title_id
ORDER BY votes DESC
LIMIT 10;
