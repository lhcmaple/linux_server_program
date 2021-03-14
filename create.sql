drop database db_webchat;
create database db_webchat;
use db_webchat;
-- 创建用户信息表
create table userinfo(
    username varchar(50) not null,
    `password` varchar(10) not null,
    primary key(username)
);
-- 创建消息表
create table messages(
    username varchar(50) not null references userinfo(username),
    content varchar(512) not null,
    mtype varchar(10) not null,
    atime date not null,
    check(mtype in('message','file'))
);
desc userinfo;
desc messages;