SELECT
	name, COUNT(*) as num_appearances
FROM
	people
INNER JOIN
	crew ON people.person_id = crew.person_id
GROUP BY name
ORDER BY num_appearances DESC
LIMIT 20;
