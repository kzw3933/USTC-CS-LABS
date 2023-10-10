import React, {useContext} from "react";
import {Navbar, Nav, NavDropdown} from "react-bootstrap";
import { LinkContainer } from "react-router-bootstrap";
import {withRouter} from "react-router-dom";

import { teacherContext } from "./App";
import {HOST, PORT, DOMAIN} from "../config"


const Header = (props) => {
    const {history} = props;
    const {
        teacher, setTeacher, auth, setAuth, admin, setAdmin
    } = useContext(teacherContext);
    const logout = async () => {
        try {
            const data = JSON.parse(await localStorage.getItem(DOMAIN));
            const res = await fetch(`${HOST}:${PORT}/api/teacher/logout`, {
                method: "POST",
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify(data)
            });
            const result = await res.json();
            if(res.ok) {
                alert(result.message);
                setAuth(false);
                setAdmin(false);
                setTeacher({});
                history.push("/");
            } else {
                alert(result.message);
            }
        } catch(err) {
            throw err;
        }
    };

    const styles = {
      avatar: {
        display: "flex", alignItems: "center"
      },
      logout: {
        color: "white", marginLeft: "10px", cursor: "pointer"
      }
    }

    const Avatar = () => (
        <div style={styles.avatar}>
            <img
             src={teacher.avatar?`/upload/${teacher.avatar}`:'/img/avatar.png'}
             alt="头像"
             width={48}
             height={48}
             className="rounded-circle"
             />
             <span style={styles.logout} onClick={() => logout()}>退出</span>
        </div>
    )
    return (
        <Navbar bg="primary" variant="dark" expand="lg">
        <Navbar.Brand href="#">USTC 教学科研登记系统</Navbar.Brand>
        <Navbar.Toggle aria-controls="basic-navbar-nav" />
        <Navbar.Collapse id="basic-navbar-nav">
          <Nav className="mr-auto">
            <LinkContainer to="/home/">
              <Nav.Link>首页</Nav.Link>
            </LinkContainer>
            {auth
              ? (
                <React.Fragment>
                    {admin ? 
                      (
                          <NavDropdown title="数据中心" id="work-nav-dropdown">
                            <LinkContainer to={`/alesson/`}>
                              <NavDropdown.Item href="#">教授课程</NavDropdown.Item>
                            </LinkContainer>
                            <LinkContainer to="/apaper/">
                              <NavDropdown.Item href="#">发表论文</NavDropdown.Item>
                            </LinkContainer>
                            <LinkContainer to="/aproject/">
                              <NavDropdown.Item href="#">承担项目</NavDropdown.Item>
                            </LinkContainer>
                          </NavDropdown>
                      ) : (
                        <React.Fragment>
                            <NavDropdown title="个人中心" id="person-nav-dropdown">
                              <LinkContainer to={`/profile/`}>
                                <NavDropdown.Item href="#">个人信息</NavDropdown.Item>
                              </LinkContainer>
                              <LinkContainer to="/setting/">
                                <NavDropdown.Item href="#">修改信息</NavDropdown.Item>
                              </LinkContainer>
                            </NavDropdown>
                            <NavDropdown title="工作空间" id="work-nav-dropdown">
                              <LinkContainer to={`/lesson/`}>
                                <NavDropdown.Item href="#">教授课程</NavDropdown.Item>
                              </LinkContainer>
                              <LinkContainer to="/paper/">
                                <NavDropdown.Item href="#">发表论文</NavDropdown.Item>
                              </LinkContainer>
                              <LinkContainer to="/project/">
                                <NavDropdown.Item href="#">承担项目</NavDropdown.Item>
                              </LinkContainer>
                            </NavDropdown>
                        </React.Fragment>
                      )
                    }
                </React.Fragment>
              )
              : null}
          </Nav>
          <div className="ml-auto">
            {auth ? <Avatar /> : null}
          </div>
        </Navbar.Collapse>
      </Navbar>
    );
};


export default withRouter(Header);