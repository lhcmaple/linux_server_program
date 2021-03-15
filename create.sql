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
    peer varchar(50) not null references userinfo(username),
    atime date not null
);
desc userinfo;
desc messages;