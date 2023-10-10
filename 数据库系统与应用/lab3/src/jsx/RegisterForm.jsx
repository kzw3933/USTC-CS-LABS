import React, { useContext } from 'react';
import {Form, Button} from "react-bootstrap";
import {HOST, PORT, DOMAIN} from "../config";
import { teacherContext } from './App';

const RegisterForm = () => {
    const { setAuth, setTeacher } = useContext(teacherContext);
    const login = async (body) => {
        try {
        const res = await fetch(`${HOST}:${PORT}/api/teacher/login`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(body),
        });
        const result = await res.json();
        if (res.ok) {
            const data = {tid: result.data.tid, tname:result.data.tname, token: result.data.token};
            await localStorage.setItem(DOMAIN,JSON.stringify(data));
            setAuth(true);
            setTeacher(result.data);
            alert(result.message);
        } else {
            alert(result.message);
        }
        } catch (err) {
            throw err.message;
        }
    };
    const register = async (body) => {
        try {
            const res = await fetch(`${HOST}:${PORT}/api/teacher`, {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify(body) 
            });
            const result = await res.json();
            if(res.ok) {
                alert(result.message);
                login(body);
            } else {
                alert(result.message);
            }
        } catch (err) {
            alert(err.message);
        };
    }
    const handleRegister = (event) => {
        event.preventDefault();
        const form = document.forms.registerForm;
        const tid = form.tid.value;
        const tname = form.tname.value;
        const gender = form.gender.value;
        const job = form.job.value;
        const password = form.password.value;
        const confirmpass = form.confirmpass.value;
        if(!tid) { alert("没有输入教师工号"); return ;}
        if(!tname) { alert("没有输入教师姓名"); return  ;}
        if(!gender) { alert("没有输入教师性别"); return  ;}
        if(!job) { alert("没有输入教师职称"); return  ;}
        if(!password) { alert("没有输入密码"); return  ;}
        if(!confirmpass) { alert("没有输入确认密码"); return  ;}
        if(password !== confirmpass) { alert("密码与确认密码不一致"); return  ;}
        const body = {tid, tname, gender, job, password};
        register(body);
    }
    return (
        <Form id="registerForm">
            <Form.Group controlId="tid">
                <Form.Label>工号</Form.Label>
                <Form.Control type="text" placeholder="请输入教师工号" />
            </Form.Group>
            <Form.Group controlId="tname">
                <Form.Label>姓名</Form.Label>
                <Form.Control type="text" placeholder="请输入教师姓名" />
            </Form.Group>
            <Form.Group controlId="gender">
                <Form.Label>性别</Form.Label>
                <Form.Control as="select">
                    <option value="1">男</option>
                    <option value="2">女</option>
                </Form.Control>
            </Form.Group>
            <Form.Group controlId="job">
                <Form.Label>职称</Form.Label>
                <Form.Control as="select">
                    <option value="0">无</option>
                    <option value="1">博士后</option>
                    <option value="2">助教</option>
                    <option value="3">讲师</option>
                    <option value="4">副教授</option>
                    <option value="5">特任教授</option>
                    <option value="6">教授</option>
                    <option value="7">助理研究员</option>
                    <option value="8">特任副研究员</option>
                    <option value="9">副研究员</option>
                    <option value="10">特任研究员</option>
                    <option value="11">研究员</option>
                </Form.Control>
            </Form.Group>
            <Form.Group controlId="password">
                <Form.Label>密码</Form.Label>
                <Form.Control type="password" placeholder="请输入密码" />
            </Form.Group>
            <Form.Group controlId="confirmpass">
                <Form.Label>确认密码</Form.Label>
                <Form.Control type="password" placeholder="请再次输入密码" />
            </Form.Group>
            <Button variant="primary" type="submit" onClick={(event) => handleRegister(event)}>注册</Button>
        </Form>
    );
}

export default RegisterForm;