SELECT
  CAST(premiered/10*10 AS TEXT) || 's' AS decade,
  ROUND(AVG(rating), 2) as avg_rating,
  MAX(rating) as top_rating,
  MIN(rating) as min_rating,
  COUNT(*) as num_releases
FROM
     titles
INNER JOIN
     ratings ON titles.title_id = ratings.title_id
WHERE premiered IS NOT NULL
GROUP BY decade
ORDER BY avg_rating DESC, decade ASC;
