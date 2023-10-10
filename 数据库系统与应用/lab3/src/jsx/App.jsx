import React, { createContext, useEffect, useState } from 'react';
import { Route, Switch } from 'react-router-dom';

import Header from './Header';
// import Footer from './Footer';
import HomePage from './HomePage';
import ProfilePage from './ProfilePage';
import SettingPage from './SettingPage';
import LessonListPage from './LessonListPage';
import PaperListPage from './PaperListPage';
import ProjectListPage from './ProjectListPage';
import ALessonListPage from './ALessonListPage';
import APaperListPage from './APaperListPage';
import AProjectListPage from './AProjectListPage';
import { HOST, PORT, DOMAIN } from '../config';

const teacherContext = createContext({
    teacher: {},
    setTeacher: () => { },
    auth: false,
    setAuth: () => { },
    admin: false,
    setAdmin: () => { },
  });

const App = () => {
    const [teacher, setTeacher] = useState({});
    const [auth, setAuth] = useState(false);
    const [admin, setAdmin] = useState(false);
    const authenticate = async () => {
        try {
            const body = await JSON.parse(localStorage.getItem(DOMAIN));
            const {tid, tname, token} = body;
            if(!tid || !token) return ;
            const res = await fetch(`${HOST}:${PORT}/api/teacher/auth`, {
              method: 'POST',
              headers: { 'Content-Type': 'application/json' },
              body: JSON.stringify(body),
            });
            const result = await res.json();
            if (res.ok) {
              setTeacher(result.data);
              setAuth(true);
              setAdmin(result.admin);
            } else {
              alert(result.message);
            }
        } catch(err) {
            throw err;
        }
    };
    useEffect(() => {
      authenticate();
    }, []);

    const styles = {
      container: {
        height: '896px',
        width: '1920px'
      },
      content: {
        marginTop: "80px",
        height: '816px',
        zIndex: 1,
      },
      header: {
        position: "fixed",
        top: 0,
        width: "100%",
        height: "80px",
        zIndex: 2
      },
    }

    return (
      <teacherContext.Provider value={{
        teacher, setTeacher, auth, setAuth, admin, setAdmin
      }}
      > 
          <div style={styles.container}>
              <div style={styles.header}>
                  <Header />
              </div>
              <div style={styles.content}>
                <Switch>
                  <Route exact path="/" component={HomePage} />
                  <Route path="/home" component={HomePage} />
                  <Route path="/profile" component={ProfilePage} />
                  <Route path="/setting" component={SettingPage} />
                  <Route path="/lesson" component={LessonListPage} />
                  <Route path="/paper" component={PaperListPage} />
                  <Route path="/project" component={ProjectListPage} />
                  <Route path="/alesson" component={ALessonListPage} />
                  <Route path="/apaper" component={APaperListPage} />
                  <Route path="/aproject" component={AProjectListPage} />
                </Switch>
              </div>
              {/* <div style={styles.footer}>
                  <Footer />
              </div> */}
          </div>
      </teacherContext.Provider>
    );
}

export { teacherContext };
export default App;