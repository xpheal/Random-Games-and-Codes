SELECT f.long_desc
	FROM
	(
		SELECT n.ndb_no as ndb_no, max(n.nutr_val) as max_val
		FROM nut_data n
	) m,
	food_des f
	WHERE m.ndb_no = f.ndb_no;