SELECT m.long_desc, max(m.ndb_count)
FROM
	(
		SELECT f.long_desc as long_desc, count(*) as ndb_count
			FROM food_des f, nut_data n
			WHERE f.ndb_no = n.ndb_no
			GROUP BY f.ndb_no
	) m;
