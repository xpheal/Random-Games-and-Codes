SELECT g.ndb_no, g.long_desc, nd.nutrdesc, n.nutr_val	
FROM
	(
		SELECT f.ndb_no as ndb_no, f.long_desc as long_desc
			FROM food_des f, nut_data n, nutr_def nd
			WHERE f.ndb_no=n.ndb_no AND n.nutr_no = nd.nutr_no AND nd.nutrdesc = "Caffeine"
		INTERSECT
		SELECT f.ndb_no as ndb_no, f.long_desc as long_desc
			FROM food_des f, nut_data n, nutr_def nd
			WHERE f.ndb_no=n.ndb_no AND n.nutr_no = nd.nutr_no AND nd.nutrdesc = "Alcohol, ethyl"
	) g,
	nut_data n,
	nutr_def nd
WHERE g.ndb_no = n.ndb_no AND n.nutr_no = nd.nutr_no AND (nd.nutrdesc = "Caffeine" OR nd.nutrdesc = "Alcohol, ethyl");


