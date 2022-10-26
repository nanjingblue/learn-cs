SELECT name,
	CASE
		WHEN died IS NOT NULL
		THEN died - born
		ELSE 2022 - born
	END AS age
FROM
	people
WHERE born >= 1900
ORDER BY age DESC, name ASC
LIMIT 20;
