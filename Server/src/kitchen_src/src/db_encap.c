#include "db_encap.h"

static char sql[SQL_STRING_LEN];
static sqlite3 *db = NULL;

sqlite3 *db_open_file(const char *path_name)
{
	sqlite3 *dbfile;
	int ret;

	ret = sqlite3_open(path_name, &dbfile);
	if(ret != SQLITE_OK){
		sqlite3_close(dbfile);
		SYS_ERR("sqlite3_open");
	}
	
	return dbfile;
}



/*
  *init database :user   inventory    order
  *
  */
void init_dbase(void)
{
	db = db_open_file(DB_FILE);

	create_db_tbl(db, USER_TABLE, USR_COLUMN);
	create_db_tbl(db, INVENTORY_TABLE, INVENTORY_COLUMN);
	create_db_tbl(db, COUNT_TABLE, COUNT_COLUMN);
	init_inventory_tbl(db, INVENTORY_TABLE, 1, "'zdivhhhkkkh'", 15, 30, 1, 1);
	init_inventory_tbl(db, INVENTORY_TABLE, 2, "'zhangkkkkkk'", 25, 30, 2, 0);
	init_inventory_tbl(db, INVENTORY_TABLE, 3, "'ddddddddddd'", 20, 30, 3, 1);
	init_inventory_tbl(db, INVENTORY_TABLE, 4, "'dddddddd'", 30, 30, 1, 1);


	Printf("insert ok!");

	return;
}

int init_inventory_tbl(
		sqlite3 *p_db, char *name_tbl, int dishid,
		char *dishname,int price, int storage, int type, int recommand)
{
	int ret;
	int result;
	sqlite3_stmt* stmt = NULL;

	sprintf(sql, \
		"insert into %s values(%d, %s, %d, %d, %d, %d)", \
		name_tbl, dishid, dishname, price, storage, type, recommand);
	result = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	DB_GETERR(result);
	
	return result;

}

void create_db_tbl(sqlite3 *p_db, char *name_tbl, char *column_define)
{
	int ret;
	sqlite3_stmt* stmt = NULL;

	if(db_is_table_exist(p_db, name_tbl)){
		return;
	}

	sprintf(sql, "create table %s(%s)", name_tbl, column_define);

	if(sqlite3_prepare_v2(p_db, sql, strlen(sql), &stmt, NULL) == SQLITE_OK){
		if(sqlite3_step(stmt) != SQLITE_DONE){
			sqlite3_finalize(stmt);
			sqlite3_close(p_db);
			SYS_ERR("sqlite3_step");
		}
	}

	sqlite3_finalize(stmt);

	return;


}

bool db_is_table_exist(sqlite3 *p_db, const char *name_tbl)
{
	sqlite3_stmt* stmt3 = NULL;

	sprintf(sql, 
			"select * from sqlite_master where type =\"table\" and name = \"%s\"", name_tbl);

	if (sqlite3_prepare_v2(p_db, sql, strlen(sql),&stmt3,NULL) != SQLITE_OK) {
		if (stmt3){
			sqlite3_finalize(stmt3);
		}
		return false;
	}

	if(sqlite3_step(stmt3) == SQLITE_ROW){
		return true;
	}

	return false;
}

int db_insert_userinfo(const char *name_tbl, usr_t *userinfo)
{
	int result;

	sprintf(sql, \
		"insert into %s values(%d, '%s', %d, '%s', %d, '%c')", \
		name_tbl, userinfo->uid, userinfo->uname, userinfo->deskid, \
		userinfo->repst_time, userinfo->uc_id, userinfo->uc_state);
	result = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	DB_GETERR(result);
	
	return result;
}


int db_get_table(char *sql, char ***db_result, int *nrow, int *ncolumn)
{
	int result;

	result = sqlite3_get_table(db, sql, db_result, nrow, ncolumn, &errmsg);
	DB_GETERR(result);

	return OK;
}

int db_excute_sql(char * sql)
{
	int result = 0;

	result = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	DB_GETERR(result);

	return OK;
}

int db_order_create(order_t *order)
{
	char sql_buf[SQL_STRING_LEN];
	char **db_ressult = NULL;
	int nrow = -1, ncolumn = -1;

	sprintf(sql_buf, \
		"select dishid,dishname,price from %s where dishid=%d and storage>=%d", \
		INVENTORY_TABLE, order->dishid, order->dishnum);
	db_get_table(sql_buf, &db_ressult, &nrow, &ncolumn);
	
	if(db_ressult != NULL){
		order->dishid = atoi(db_ressult[ncolumn]);
		strncpy(order->dishname, db_ressult[ncolumn+1], NAMESIZ);
		order->dishprice = atoi(db_ressult[ncolumn+2]);

		sprintf(sql_buf, \
			"update %s set storage=storage-%d where dishid=%d", \
			INVENTORY_TABLE, order->dishnum, order->dishid);
		db_excute_sql(sql_buf);

		return OK;
	}
	else
		return FAIL;
}

void db_order_insert_counttbl(order_t *fin_ord, int cookerid)
{
	char sql_buf[SQL_STRING_LEN];

	sprintf(sql_buf, \
		"insert into \
		%s(uc_id, dishid, dishname, dishprice, dishnum, all_price, cookerid, booktime, donetime) \
		values(%d,%d,'%s',%d,%d,%d,%d,'%s','%s')", \
		COUNT_TABLE, fin_ord->uc_id, fin_ord->dishid, fin_ord->dishname, \
		fin_ord->dishprice, fin_ord->dishnum, fin_ord->all_price, cookerid, fin_ord->booktime, fin_ord->donetime);
	db_excute_sql(sql_buf);
}



