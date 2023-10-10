import React, { useEffect, useState, useContext } from "react";
import {Form, Button} from "react-bootstrap";
import {HOST, PORT, DOMAIN} from "../config.js";
import { teacherContext } from "./App";

const SettingForm = () => {
    const {setTeacher} = useContext(teacherContext);
    const [settings, setSettings] = useState([]);
    const loadSettings = async () => {
        try {
            const storage = await localStorage.getItem(DOMAIN);
            const {tid, tname, token} = JSON.parse(storage);
            const res = await fetch(`${HOST}:${PORT}/api/teacher/${tid}`, {method: 'GET'});
            const result = await res.json();
            if(res.ok) {
                setSettings(result.data);
                setTeacher(result.data);
            } else {
                alert(result.message);
            }
        } catch(err) {
            alert(err.message);
        }
    };
    useEffect(() => {
        loadSettings();
    }, []);
    const setting = async () => {
        try {
            const {tid, tname, token} = JSON.parse(await localStorage.getItem(DOMAIN));
            const formData = new FormData();
            await formData.append('tid', tid);
            await formData.append('tname', tname);
            await formData.append('token', token);
            await formData.append('avatar', document.querySelector('#uploader').files[0]);
            const res = await fetch(`${HOST}:${PORT}/api/teacher`, {method: 'PATCH', body: formData});
            const result = await res.json();
            if(res.ok) {
                alert(result.message);
                loadSettings();
            } else {
                alert(result.message);
            }
        } catch (err) {
            alert(err.message);
        }
    }
    const handleSetting = (err) => {
        err.preventDefault();
        setting();
    }

    const styles = {
        img: {width: '150px', height: '150px'}
    }

    return (
        <Form id="settingForm">
            <Form.Group controlId="uploader">
                <Form.Label>头像</Form.Label><br/>
                <img 
                 src={settings.avatar?`/upload/${settings.avatar}`:'/img/avatar.png'}
                 alt="头像"
                 className="rounded-circle img-thumbnail"
                 style={styles.img}
                 />
                <Form.Control type="file" placeholder="请上传头像" className="mt-3"/> 
            </Form.Group>
            <Button variant="primary" type="submit" onClick={(event) => handleSetting(event)}>确定</Button>
        </Form>
    )
}

export default SettingForm;