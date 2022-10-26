SELECT
	primary_title, premiered, CAST(runtime_minutes AS VARCHAR) || " (mins)"
FROM
	titles
WHERE genres LIKE '%Sci-Fi%'
ORDER BY runtime_minutes DESC
LIMIT 10;
