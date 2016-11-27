begin;
use s5;

/** 删除超过日期的邮件*/
delete  from s1.Mail where s1.Mail.sendTime+(3600*24*10)<  (SELECT UNIX_TIMESTAMP(CURRENT_TIMESTAMP));
delete  from s2.Mail where s2.Mail.sendTime+(3600*24*10)<  (SELECT UNIX_TIMESTAMP(CURRENT_TIMESTAMP));
delete  from s3.Mail where s3.Mail.sendTime+(3600*24*10)<  (SELECT UNIX_TIMESTAMP(CURRENT_TIMESTAMP));
delete  from s4.Mail where s4.Mail.sendTime+(3600*24*10)<  (SELECT UNIX_TIMESTAMP(CURRENT_TIMESTAMP));
/** 修改邮件表的id*/
alter table s2.Mail change s2.Mail.id id BIGINT NOT NULL;
alter table s2.Mail drop primary key;
alter table s3.Mail change s3.Mail.id id BIGINT NOT NULL;
alter table s3.Mail drop primary key;
alter table s4.Mail change s4.Mail.id id BIGINT NOT NULL;
alter table s4.Mail drop primary key;
select @maxMailId:=max(s1.Mail.id) from s1.Mail;
update s2.Mail set s2.Mail.id = @maxMailId + s2.Mail.id;
select @maxMailId:=max(s2.Mail.id) from s2.Mail;
update s3.Mail set s3.Mail.id = @maxMailId + s3.Mail.id;
select @maxMailId:=max(s3.Mail.id) from s3.Mail;
update s4.Mail set s4.Mail.id = @maxMailId + s4.Mail.id;


/** 删除 小于30并且没有充值并且2周没有登录的玩家 **/ 
delete  from s1.Role where s1.Role.level<30 and s1.Role.paidMoney=0 and ( s1.Role.logoutTime+(3600*24*14)<  (SELECT UNIX_TIMESTAMP(CURRENT_TIMESTAMP)));
delete  from s2.Role where s2.Role.level<30 and s2.Role.paidMoney=0 and ( s2.Role.logoutTime+(3600*24*14)<  (SELECT UNIX_TIMESTAMP(CURRENT_TIMESTAMP)));
delete  from s3.Role where s3.Role.level<30 and s3.Role.paidMoney=0 and ( s3.Role.logoutTime+(3600*24*14)<  (SELECT UNIX_TIMESTAMP(CURRENT_TIMESTAMP)));
delete  from s4.Role where s4.Role.level<30 and s4.Role.paidMoney=0 and ( s4.Role.logoutTime+(3600*24*14)<  (SELECT UNIX_TIMESTAMP(CURRENT_TIMESTAMP)));

/** 账户表 删除重复的帐号*/
/** 1服跟2服重复 删2服帐号*/
drop temporary table if exists  accTemp;
create temporary table accTemp( select s2.Account.accountName from s1.Account,s2.Account where s1.Account.accountName=s2.Account.accountName);
delete from s2.Account where s2.Account.accountName in(select * from accTemp);

/** 1服跟3服重复 删3服帐号*/
drop temporary table if exists  accTemp;
create temporary table accTemp( select s3.Account.accountName from s1.Account,s3.Account where s1.Account.accountName=s3.Account.accountName);
delete from s3.Account where s3.Account.accountName in(select * from accTemp);

/** 1服跟4服重复 删4服帐号*/
drop temporary table if exists  accTemp;
create temporary table accTemp( select s4.Account.accountName from s1.Account,s4.Account where s1.Account.accountName=s4.Account.accountName);
delete from s4.Account where s4.Account.accountName in(select * from accTemp);

/** 2服跟3服重复 删3服帐号*/
drop temporary table if exists  accTemp;
create temporary table accTemp( select s3.Account.accountName from s2.Account,s3.Account where s2.Account.accountName=s3.Account.accountName);
delete from s3.Account where s3.Account.accountName in(select * from accTemp);

/** 2服跟4服重复 删4服帐号*/
drop temporary table if exists  accTemp;
create temporary table accTemp( select s2.Account.accountName from s2.Account,s4.Account where s2.Account.accountName=s4.Account.accountName);
delete from s4.Account where s4.Account.accountName in(select * from accTemp);

/** 3服跟4服重复 删4服帐号*/
drop temporary table if exists  accTemp;
create temporary table accTemp( select s3.Account.accountName from s3.Account,s4.Account where s3.Account.accountName=s4.Account.accountName);
delete from s4.Account where s4.Account.accountName in(select * from accTemp);
/** 删除账户表主键 修改账户id*/
alter table s2.Account change s2.Account.accountId accountId BIGINT NOT NULL;
alter table s2.Account drop primary key;
alter table s3.Account change s3.Account.accountId accountId BIGINT NOT NULL;
alter table s3.Account drop primary key;
alter table s4.Account change s4.Account.accountId accountId BIGINT NOT NULL;
alter table s4.Account drop primary key;
select @maxAccountId:=max(s1.Account.accountId) from s1.Account;
update s2.Account set s2.Account.accountId = @maxAccountId + s2.Account.accountId;
select @maxAccountId:=max(s2.Account.accountId) from s2.Account;
update s3.Account set s3.Account.accountId = @maxAccountId + s3.Account.accountId;
select @maxAccountId:=max(s3.Account.accountId) from s3.Account;
update s4.Account set s4.Account.accountId = @maxAccountId + s4.Account.accountId;

/** 设置玩家的serverid*/
update s1.Role set s1.Role.serverId=4;
update s2.Role set s2.Role.serverId=5;
update s3.Role set s3.Role.serverId=6;
update s4.Role set s4.Role.serverId=7;

/** 处理重名玩家 */
/** 1服 跟2服重名 2服角色名 邮件表receiver 前加s2.*/
drop temporary table if exists  temp;
create temporary table temp( select s2.Role.roleName  from s1.Role,s2.Role where s1.Role.roleName=s2.Role.roleName);
update s2.Role set s2.Role.roleName=concat("s2.",s2.Role.roleName) where s2.Role.roleName in (select * from temp);
update s2.Mail set s2.Mail.receiver=concat("s2.",s2.Mail.receiver) where s2.Mail.receiver in (select * from temp);
update s2.Market set s2.Market.seller = concat("s2.", s2.Market.seller) where s2.Market.seller in (select * from temp);
update s2.DuelRank set s2.DuelRank.roleName = concat("s2.", s2.DuelRank.roleName) where s2.DuelRank.roleName in (select * from temp);
update s2.Guild set s2.Guild.masterName = concat("s2.", s2.Guild.masterName) where s2.Guild.masterName in (select * from temp);
update s2.GuildMember set s2.GuildMember.roleName = concat("s2.", s2.GuildMember.roleName) where s2.GuildMember.roleName in (select * from temp);

/** 1服 跟3服重名 3服角色名 邮件表receiver 前加s3.*/
drop temporary table if exists temp;
create temporary table temp( select s3.Role.roleName  from s1.Role,s3.Role where s1.Role.roleName=s3.Role.roleName);
update s3.Role set s3.Role.roleName=concat("s3.",s3.Role.roleName) where s3.Role.roleName in (select * from temp);
update s3.Mail set s3.Mail.receiver=concat("s3.",s3.Mail.receiver) where s3.Mail.receiver in (select * from temp);
update s3.Market set s3.Market.seller = concat("s3.", s3.Market.seller) where s3.Market.seller in (select * from temp);
update s3.DuelRank set s3.DuelRank.roleName = concat("s3.", s3.DuelRank.roleName) where s3.DuelRank.roleName in (select * from temp);
update s3.Guild set s3.Guild.masterName = concat("s3.", s3.Guild.masterName) where s3.Guild.masterName in (select * from temp);
update s3.GuildMember set s3.GuildMember.roleName = concat("s3.", s3.GuildMember.roleName) where s3.GuildMember.roleName in (select * from temp);

/** 1服 跟4服重名 4服角色名 邮件表receiver 前加s4.*/
drop temporary table if exists temp;
create temporary table temp( select s4.Role.roleName  from s1.Role,s4.Role where s1.Role.roleName=s4.Role.roleName);
update s4.Role set s4.Role.roleName=concat("s4.",s4.Role.roleName) where s4.Role.roleName in (select * from temp);
update s4.Mail set s4.Mail.receiver=concat("s4.",s4.Mail.receiver) where s4.Mail.receiver in (select * from temp);
update s4.Market set s4.Market.seller = concat("s4.", s4.Market.seller) where s4.Market.seller in (select * from temp);
update s4.DuelRank set s4.DuelRank.roleName = concat("s4.", s4.DuelRank.roleName) where s4.DuelRank.roleName in (select * from temp);
update s4.Guild set s4.Guild.masterName = concat("s4.", s4.Guild.masterName) where s4.Guild.masterName in (select * from temp);
update s4.GuildMember set s4.GuildMember.roleName = concat("s4.", s4.GuildMember.roleName) where s4.GuildMember.roleName in (select * from temp);

/** 2服 跟3服重名 3服角色名 邮件表receiver 前加s3.*/
drop temporary table if exists temp;
create temporary table temp( select s3.Role.roleName  from s2.Role,s3.Role where s2.Role.roleName=s3.Role.roleName);
update s3.Role set s3.Role.roleName=concat("s3.",s3.Role.roleName) where s3.Role.roleName in (select * from temp);
update s3.Mail set s3.Mail.receiver=concat("s3.",s3.Mail.receiver) where s3.Mail.receiver in (select * from temp);
update s3.Market set s3.Market.seller = concat("s3.", s3.Market.seller) where s3.Market.seller in (select * from temp);
update s3.DuelRank set s3.DuelRank.roleName = concat("s3.", s3.DuelRank.roleName) where s3.DuelRank.roleName in (select * from temp);
update s3.Guild set s3.Guild.masterName = concat("s3.", s3.Guild.masterName) where s3.Guild.masterName in (select * from temp);
update s3.GuildMember set s3.GuildMember.roleName = concat("s3.", s3.GuildMember.roleName) where s3.GuildMember.roleName in (select * from temp);

/** 2服 跟4服重名 4服角色名 邮件表receiver 前加s4.*/
drop temporary table if exists temp;
create temporary table temp( select s4.Role.roleName  from s2.Role,s4.Role where s2.Role.roleName=s4.Role.roleName);
update s4.Role set s4.Role.roleName=concat("s4.",s4.Role.roleName) where s4.Role.roleName in (select * from temp);
update s4.Mail set s4.Mail.receiver=concat("s4.",s4.Mail.receiver) where s4.Mail.receiver in (select * from temp);
update s4.Market set s4.Market.seller = concat("s4.", s4.Market.seller) where s4.Market.seller in (select * from temp);
update s4.DuelRank set s4.DuelRank.roleName = concat("s4.", s4.DuelRank.roleName) where s4.DuelRank.roleName in (select * from temp);
update s4.Guild set s4.Guild.masterName = concat("s4.", s4.Guild.masterName) where s4.Guild.masterName in (select * from temp);
update s4.GuildMember set s4.GuildMember.roleName = concat("s4.", s4.GuildMember.roleName) where s4.GuildMember.roleName in (select * from temp);

/** 3服 跟4服重名 3服角色名 邮件表receiver 前加s3.*/
drop temporary table if exists temp;
create temporary table temp( select s4.Role.roleName  from s3.Role,s4.Role where s3.Role.roleName=s4.Role.roleName);
update s3.Role set s3.Role.roleName=concat("s3.",s3.Role.roleName) where s3.Role.roleName in (select * from temp);
update s3.Mail set s3.Mail.receiver=concat("s3.",s3.Mail.receiver) where s3.Mail.receiver in (select * from temp);
update s3.Market set s3.Market.seller = concat("s3.", s3.Market.seller) where s3.Market.seller in (select * from temp);
update s3.DuelRank set s3.DuelRank.roleName = concat("s3.", s3.DuelRank.roleName) where s3.DuelRank.roleName in (select * from temp);
update s3.Guild set s3.Guild.masterName = concat("s3.", s3.Guild.masterName) where s3.Guild.masterName in (select * from temp);
update s3.GuildMember set s3.GuildMember.roleName = concat("s3.", s3.GuildMember.roleName) where s3.GuildMember.roleName in (select * from temp);

/** 删除 只有一个玩家的帮派*/
alter table s2.Guild drop primary key;
alter table s3.Guild drop primary key;
alter table s4.Guild drop primary key;

drop temporary table if exists guildTemp;
create temporary table guildTemp (SELECT s1.GuildMember.guildId FROM s1.GuildMember GROUP BY s1.GuildMember.guildId HAVING count(*)=1);
delete from s1.GuildMember where s1.GuildMember.guildId in(select * from guildTemp);
delete from s1.Guild where s1.Guild.guildId in(select * from guildTemp);

/** 2服帮派id 加上1服帮派id的最大值 防止id重复*/
select @maxGuildId:=max(s1.Guild.guildId) from s1.Guild;
update s2.Guild set s2.Guild.guildId=@maxGuildId+s2.Guild.guildId;
update s2.GuildMember set s2.GuildMember.guildId = @maxGuildId + s2.GuildMember.guildId;


drop temporary table if exists guildTemp;
create temporary table guildTemp (SELECT s2.GuildMember.guildId FROM s2.GuildMember GROUP BY s2.GuildMember.guildId HAVING count(*)=1);
delete from s2.GuildMember where s2.GuildMember.guildId in(select * from guildTemp);
delete from s2.Guild where s2.Guild.guildId in(select * from guildTemp);
/** 3服帮派id 加上2服帮派id的最大值 防止id重复*/
select @maxGuildId:=max(s2.Guild.guildId) from s2.Guild;
update s3.Guild set s3.Guild.guildId=@maxGuildId+s3.Guild.guildId;
update s3.GuildMember set s3.GuildMember.guildId = @maxGuildId + s3.GuildMember.guildId;

drop temporary table if exists guildTemp;
create temporary table guildTemp (SELECT s3.GuildMember.guildId FROM s3.GuildMember GROUP BY s3.GuildMember.guildId HAVING count(*)=1);
delete from s3.GuildMember where s3.GuildMember.guildId in(select * from guildTemp);
delete from s3.Guild where s3.Guild.guildId in(select * from guildTemp);

/** 4服帮派id 加上3服帮派id的最大值 防止id重复*/
select @maxGuildId:=max(s3.Guild.guildId) from s3.Guild;
update s4.Guild set s4.Guild.guildId=@maxGuildId+s4.Guild.guildId;
update s4.GuildMember set s4.GuildMember.guildId = @maxGuildId + s4.GuildMember.guildId;

drop temporary table if exists guildTemp;
create temporary table guildTemp (SELECT s4.GuildMember.guildId FROM s4.GuildMember GROUP BY s4.GuildMember.guildId HAVING count(*)=1);
delete from s4.GuildMember where s4.GuildMember.guildId in(select * from guildTemp);
delete from s4.Guild where s4.Guild.guildId in(select * from guildTemp);


 /** 处理帮派重名 */
/** 1服和2服帮派重名处理:2服帮派名前加s2. */
drop temporary table if exists guildTemp;
create temporary table guildTemp ( select s2.Guild.guildName from s1.Guild,s2.Guild where s1.Guild.guildName = s2.Guild.guildName);
update s2.Guild set s2.Guild.guildName = concat("s2.", s2.Guild.guildName) where s2.Guild.guildName in (select * from guildTemp);
/** 1服和3服帮派重名处理:3服帮派名前加s3. */
drop temporary table if exists guildTemp;
create temporary table guildTemp ( select s3.Guild.guildName from s1.Guild,s3.Guild where s1.Guild.guildName = s3.Guild.guildName);
update s3.Guild set s3.Guild.guildName = concat("s3.", s3.Guild.guildName) where s3.Guild.guildName in (select * from guildTemp);
/** 1服和4服帮派重名处理:4服帮派名前加s4. */
drop temporary table if exists guildTemp;
create temporary table guildTemp ( select s4.Guild.guildName from s1.Guild,s4.Guild where s1.Guild.guildName = s4.Guild.guildName);
update s4.Guild set s4.Guild.guildName = concat("s4.", s4.Guild.guildName) where s4.Guild.guildName in (select * from guildTemp);
/** 2服和3服帮派重名处理:3服帮派名前加s3. */
drop temporary table if exists guildTemp;
create temporary table guildTemp ( select s3.Guild.guildName from s2.Guild,s3.Guild where s2.Guild.guildName = s3.Guild.guildName);
update s3.Guild set s3.Guild.guildName = concat("s3.", s3.Guild.guildName) where s3.Guild.guildName in (select * from guildTemp);
/** 2服和4服帮派重名处理:4服帮派名前加s4. */
drop temporary table if exists guildTemp;
create temporary table guildTemp ( select s4.Guild.guildName from s2.Guild,s4.Guild where s2.Guild.guildName = s4.Guild.guildName);
update s4.Guild set s4.Guild.guildName = concat("s4.", s4.Guild.guildName) where s4.Guild.guildName in (select * from guildTemp);
/** 3服和4服帮派重名处理:3服帮派名前加s3. */
drop temporary table if exists guildTemp;
create temporary table guildTemp ( select s3.Guild.guildName from s3.Guild,s4.Guild where s3.Guild.guildName = s4.Guild.guildName);
update s3.Guild set s3.Guild.guildName = concat("s3.", s3.Guild.guildName) where s3.Guild.guildName in (select * from guildTemp);


/** 武考榜 删除表的主键*/
alter table s1.DuelRank drop primary key;
alter table s2.DuelRank drop primary key;
alter table s3.DuelRank drop primary key;
alter table s4.DuelRank drop primary key;
/** 设置 rankId id*/
update s1.DuelRank set s1.DuelRank.rankId=s1.DuelRank.rankId*3+s1.DuelRank.rankId;
update s2.DuelRank set s2.DuelRank.rankId=s2.DuelRank.rankId*3+s2.DuelRank.rankId+1;
update s3.DuelRank set s3.DuelRank.rankId=s3.DuelRank.rankId*3+s3.DuelRank.rankId+2;
update s4.DuelRank set s4.DuelRank.rankId=s4.DuelRank.rankId*3+s4.DuelRank.rankId+3;

 /** 交易recordId处理 **/
/** 2服交易recordId加上1服最大交易recordId **/
select @maxRecordId:=max(s1.Market.recordId) from s1.Market;
update s2.Market set s2.Market.recordId = @maxRecordId + s2.Market.recordId;
/** 3服交易recordId加上2服最大交易recordId **/
select @maxRecordId:=max(s2.Market.recordId) from s2.Market;
update s3.Market set s3.Market.recordId = @maxRecordId + s3.Market.recordId;
/** 服交易recordId加上2服最大交易recordId **/
select @maxRecordId:=max(s3.Market.recordId) from s3.Market;
update s4.Market set s4.Market.recordId = @maxRecordId + s4.Market.recordId;

/* 合并表 s1 s2 s3 s4 合入s5中*/
/** 账号表*/
insert into s5.Account select * from s1.Account;
insert into s5.Account select * from s2.Account;
insert into s5.Account select * from s3.Account;
insert into s5.Account select * from s4.Account;
/** 角色表*/
insert into s5.Role select * from s1.Role;
insert into s5.Role select * from s2.Role;
insert into s5.Role select * from s3.Role;
insert into s5.Role select * from s4.Role;
update s5.Role set s5.Role.COM_SecretPassage = 0;
/** 帮派*/
insert into s5.Guild select * from s1.Guild;
insert into s5.Guild select * from s2.Guild;
insert into s5.Guild select * from s3.Guild;
insert into s5.Guild select * from s4.Guild;
update s5.Guild set s5.Guild.requestList = 0;
insert into s5.GuildMember select * from s1.GuildMember;
insert into s5.GuildMember select * from s2.GuildMember;
insert into s5.GuildMember select * from s3.GuildMember;
insert into s5.GuildMember select * from s4.GuildMember;
/** 邮件*/
insert into s5.Mail select * from s1.Mail;
insert into s5.Mail select * from s2.Mail;
insert into s5.Mail select * from s3.Mail;
insert into s5.Mail select * from s4.Mail;
/** 交易 */
insert into s5.Market select * from s1.Market;
insert into s5.Market select * from s2.Market;
insert into s5.Market select * from s3.Market;
insert into s5.Market select * from s4.Market;
/** 排行榜*/
insert into s5.DuelRank select * from s1.DuelRank;
insert into s5.DuelRank select * from s2.DuelRank;
insert into s5.DuelRank select * from s3.DuelRank;
insert into s5.DuelRank select * from s4.DuelRank;
delete  from s5.DuelRank where s5.DuelRank.rankId>2999;
commit;
