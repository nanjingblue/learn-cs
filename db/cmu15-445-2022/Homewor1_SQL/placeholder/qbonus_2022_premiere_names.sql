

WITH t AS (
      SELECT
            titles.primary_title as name,
            akas.title as dubbed,
            (titles.primary_title = akas.title) AS starter
      FROM
            titles
            INNER JOIN akas ON titles.title_id = akas.title_id
            INNER JOIN ratings ON titles.title_id = ratings.title_id
            WHERE titles.premiered = 2022 AND titles.type = 'tvSeries' AND ratings.rating >= 9.0
            GROUP BY titles.primary_title, akas.title
            ORDER BY titles.primary_title ASC, starter DESC, akas.title ASC
),
r AS (
      select t.name as name, row_number() over (order by t.name asc) as seqnum, t.dubbed as dubbed
      from t
),
dub_cte AS (
      SELECT
            r.name, r.seqnum, r.dubbed
      FROM
            r
      WHERE
            r.name = r.dubbed
      UNION ALL
      SELECT
            r.name, r.seqnum,  c.dubbed || ', ' || r.dubbed
      FROM
            r INNER JOIN dub_cte c ON r.name = c.name AND r.seqnum = c.seqnum + 1
)
SELECT name, MAX(dubbed) FROM dub_cte
GROUP BY name
;

