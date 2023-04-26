for i in `ls *.tbl`; do
  table=${i/.tbl/}
  echo "Loading $table..."
  sed 's/|$//' $i > /tmp/$i
  psql tpch-test -q -c "TRUNCATE $table"
  psql tpch-test -c "\\copy $table FROM '/tmp/$i' CSV DELIMITER '|'"
done