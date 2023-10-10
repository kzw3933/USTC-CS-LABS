import React, { useContext } from 'react';
import {Form, Button} from "react-bootstrap";
import { teacherContext } from './App';
import {HOST, PORT, DOMAIN} from "../config";


const LoginForm = () => {
    const {setTeacher, setAuth, setAdmin} = useContext(teacherContext);
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
                setAdmin(result.admin);
                setTeacher(result.data);
                alert(result.message);
            } else {
                alert(result.message);
            }
        } catch (err) {
            throw err.message;
        }
    };
    const handleLogin = (event) => {
        event.preventDefault();
        const form = document.forms.loginForm;
        const tid = form.tid.value;
        const password = form.password.value;
        if(!tid) { alert("没有输入教师工号"); return ; }
        if(!password) { alert("没有输入密码"); return ; }
        const body = {tid, password};
        login(body);
    }
    return (
        <Form id="loginForm">
            <Form.Group controlId="tid">
                <Form.Label>教师工号</Form.Label>
                <Form.Control type="text" placeholder="请输入教师工号" />
            </Form.Group>
            <Form.Group controlId="password">
                <Form.Label>密码</Form.Label>
                <Form.Control type="password" placeholder="请输入密码" />
            </Form.Group>
            <Button variant="primary" type="submit" onClick={(event) => handleLogin(event)}>登录</Button>
        </Form>
    );
}

export default LoginForm;