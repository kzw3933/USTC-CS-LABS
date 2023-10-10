import React, { useState, useContext } from 'react';
import { Container, Row, Col, Tabs, Tab, Card } from 'react-bootstrap';

import { Demo, Slogan } from './Introduction';
import RegisterForm from './RegisterForm';
import LoginForm from './LoginForm';
import { teacherContext } from './App';

const RegLogForm = () => {
    const [mode, setMode] = useState('login');
    return (
        <Tabs
            id="homeTabs"
            activeKey={mode}
            onSelect={mode => setMode(mode)}
        >
            <Tab eventKey="login" title="登陆">
                <Card>
                    <Card.Body>
                        <LoginForm />
                    </Card.Body>
                </Card> 
            </Tab>
            <Tab eventKey="register" title="注册">
                <Card>
                    <Card.Body>
                        <RegisterForm />
                    </Card.Body>
                </Card>
            </Tab>
        </Tabs>
    )
}

const HomePage = () => {
  const { auth } = useContext(teacherContext);

  const styles = {
    homepage: {
      display: "flex",
      alignItems: "center",
      justifyContent: "center"
    },
    div: {
      width: "100%",
    },
    reglog: {
      marginLeft: "700px",
      marginRight: "700px"
    }
  }
 return (
    <Container fluid style={styles.homepage}>
      <div style={styles.div}>
        {auth ? (
                <Slogan />
              ) : (
                <div style={styles.reglog}>
                    <RegLogForm />
                </div>
              )}
      </div>
    </Container>
  );
};

export default HomePage;
