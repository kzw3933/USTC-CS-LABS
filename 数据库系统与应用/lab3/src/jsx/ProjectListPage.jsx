import React, { useState, useEffect, useContext } from 'react';
import { Button, Container, Table, Modal, Form, Col } from 'react-bootstrap';

import { HOST, PORT, DOMAIN } from '../config';
import { teacherContext } from './App';

const PostForm = () => {
    return (
      <Form id="postForm">
          <Form.Group controlId="pid">
              <Form.Label>项目号</Form.Label>
              <Form.Control type="text" placeholder="请输入项目号"/>
          </Form.Group>
          <Form.Group controlId="pname">
              <Form.Label>项目名称</Form.Label>
              <Form.Control type="text" placeholder="请输入项目名称"/>
          </Form.Group>
          <Form.Group controlId="projectSource">
              <Form.Label>项目来源</Form.Label>
              <Form.Control type="text" placeholder="请输入发表源"/>
          </Form.Group>
          <Form.Group controlId="projectType">
              <Form.Label>项目类型</Form.Label>
              <Form.Control as="select">
                <option value="">无</option>
                <option value="1">国家级项目</option>
                <option value="2">省部级项目</option>
                <option value="3">市厅级项目</option>
                <option value="4">企业合作项目</option>
                <option value="5">其他类型项目</option>
              </Form.Control>
          </Form.Group>
          <Form.Group controlId="totalBudget">
              <Form.Label>总经费</Form.Label>
              <Form.Control type="text" placeholder="请输入项目总经费"/>
          </Form.Group>
          <Form.Group controlId="startYear">
              <Form.Label>开始年份</Form.Label>
              <Form.Control type="text" placeholder="请输入开始年份"/>
          </Form.Group>
          <Form.Group controlId="endYear">
              <Form.Label>结束年份</Form.Label>
              <Form.Control type="text" placeholder="请输入结束年份"/>
          </Form.Group>
          <Form.Group controlId="projectRank">
              <Form.Label>排名</Form.Label>
              <Form.Control type="text" placeholder="请输入排名"/>
          </Form.Group>
          <Form.Group controlId="undertakeFunding">
              <Form.Label>承担经费</Form.Label>
              <Form.Control type="text" placeholder="请输入承担经费"/>
          </Form.Group>
      </Form>
    );
}

const PostButton = (props) => {
    const {loadProjects} = props;
    const [show, setShow] = useState(false);
    const showModal = () => setShow(true);
    const closeModal = () => setShow(false);
    const post = async (body) => {
        try {

            if(body.totalBudget !== body.undertakeFunding) {
                throw new Error('普通用户只支持新增个人项目(个人承担经费要等于项目总经费)');
            }

            const res = await fetch(`${HOST}:${PORT}/api/project`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(body)
            }) ;
            const result = await res.json();
            if(res.ok) {
                alert(result.message);
                closeModal();
                loadProjects();
            } else {
                alert(result.message);
            }
        } catch(err) {
            alert(err.message);
        } 
    }
    const handlePost = async () => {
        const {tid} = JSON.parse(await localStorage.getItem(DOMAIN));
        const form = document.forms.postForm;
        const pid = form.pid.value;
        const pname = form.pname.value;
        const projectSource = form.projectSource.value;
        const projectType = form.projectType.value;
        const totalBudget = form.totalBudget.value;
        const startYear = form.startYear.value;
        const endYear = form.endYear.value;
        const projectRank = form.projectRank.value;
        const undertakeFunding = form.undertakeFunding.value;
        const body = { 
          pid, pname, projectSource, projectType, totalBudget, startYear, endYear, projectRank, undertakeFunding, tid 
        };
        post(body);
    }
    return (
        <div>
            <Button variant="outline-success" size="sm" onClick={() => showModal()}>
                新建项目
            </Button>
            <Modal show={show} onHide={() => closeModal()}>
                <Modal.Header closeButton>
                    <Modal.Title>新建项目</Modal.Title>
                </Modal.Header>
                <Modal.Body>
                    <PostForm/>
                </Modal.Body>
                <Modal.Footer>
                    <Button variant="secondary" onClick={() => closeModal() }>
                        关闭
                    </Button>
                    <Button variant="primary" onClick={() => handlePost()}>
                        提交
                    </Button>
                </Modal.Footer>
            </Modal>
        </div>
    );
}

const ModifyForm = (props) => {
    const { modifyPid, setModifyPid } = props;
    const {tid} = JSON.parse(localStorage.getItem(DOMAIN));

    const handleModifyPidChange = (event) => {
      setModifyPid(event.target.value);
    };
    const handleButtonClick = async () => {
      try {
        const res = await fetch(`${HOST}:${PORT}/api/project/${tid}/${modifyPid}`, { method: 'GET' });
        const result = await res.json();
        if (res.ok) {
          const form = document.forms.modifyForm;
          form.pid.value = result.data.pid;
          form.pname.value = result.data.pname;
          form.projectSource.value = result.data.projectSource;
          form.projectType.value = result.data.projectType;
          form.totalBudget.value = result.data.totalBudget;
          form.startYear.value = result.data.startYear;
          form.endYear.value = result.data.endYear;
          form.projectRank.value = result.data.projectRank;
          form.undertakeFunding.value = result.data.undertakeFunding;
        } else {
          alert(result.message);
        }
      } catch (err) {
        alert(err.message);
      }
    };

    return (
      <Form id="modifyForm">
          <Form.Group controlId="pid">
            <Form.Label>待修改项目的项目号</Form.Label>
            <div className="d-flex">
              <Form.Control type="text" placeholder="请输入待修改项目的项目号" value={modifyPid} onChange={handleModifyPidChange} />
              <Button variant="primary" className="ml-2" onClick={handleButtonClick}>载入</Button>
            </div>
          </Form.Group>
          <Form.Group controlId="pname">
              <Form.Label>项目名称</Form.Label>
              <Form.Control type="text" placeholder="请输入项目名称"/>
          </Form.Group>
          <Form.Group controlId="projectSource">
              <Form.Label>项目来源</Form.Label>
              <Form.Control type="text" placeholder="请输入发表源"/>
          </Form.Group>
          <Form.Group controlId="projectType">
              <Form.Label>项目类型</Form.Label>
              <Form.Control as="select">
                <option value="">无</option>
                <option value="1">国家级项目</option>
                <option value="2">省部级项目</option>
                <option value="3">市厅级项目</option>
                <option value="4">企业合作项目</option>
                <option value="5">其他类型项目</option>
              </Form.Control>
          </Form.Group>
          <Form.Group controlId="totalBudget">
              <Form.Label>总经费</Form.Label>
              <Form.Control type="text" placeholder="请输入项目总经费"/>
          </Form.Group>
          <Form.Group controlId="startYear">
              <Form.Label>开始年份</Form.Label>
              <Form.Control type="text" placeholder="请输入开始年份"/>
          </Form.Group>
          <Form.Group controlId="endYear">
              <Form.Label>开始年份</Form.Label>
              <Form.Control type="text" placeholder="请输入结束年份"/>
          </Form.Group>
          <Form.Group controlId="projectRank">
              <Form.Label>排名</Form.Label>
              <Form.Control type="text" placeholder="请输入排名"/>
          </Form.Group>
          <Form.Group controlId="undertakeFunding">
              <Form.Label>承担经费</Form.Label>
              <Form.Control type="text" placeholder="请输入承担经费"/>
          </Form.Group>
      </Form>
    );
}

const ModifyButton = (props) => {
    const { loadProjects } = props;
    const [show, setShow] = useState(false);
    const showModal = () => setShow(true);
    const closeModal = () => setShow(false);
    const [modifyPid, setModifyPid] = useState("");
    const modify = async (body) => {
      try {
        if(body.totalBudget !== body.undertakeFunding) {
          throw new Error('普通用户只支持修改个人项目(个人承担经费要等于项目总经费)');
        }
        const res = await fetch(`${HOST}:${PORT}/api/project`, {
          method: 'PATCH',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify(body),
        });
        const result = await res.json();
        if (res.ok) {
          alert(result.message);
          closeModal();
          loadProjects();
        } else {
          alert(result.message);
        }
      } catch (err) {
        alert(err.message);
      }
    };
    const handleModify = async () => {
      const { tid } = JSON.parse(await localStorage.getItem(DOMAIN));
      const form = document.forms.modifyForm;
      const pname = form.pname.value;
      const projectSource = form.projectSource.value;
      const projectType = form.projectType.value;
      const totalBudget = form.totalBudget.value;
      const startYear = form.startYear.value;
      const endYear = form.endYear.value;
      const projectRank = form.projectRank.value;
      const undertakeFunding = form.undertakeFunding.value;
      const body = { 
        modifyPid, pname, projectSource, projectType, totalBudget, startYear, endYear, projectRank, undertakeFunding, tid 
      };
      modify(body);
    };
    return (
      <div>
        <Button variant="outline-warning" size="sm" onClick={() => showModal()}>
                  修改
        </Button>
        <Modal show={show} onHide={() => closeModal()}>
          <Modal.Header closeButton>
            <Modal.Title>修改项目</Modal.Title>
          </Modal.Header>
          <Modal.Body>
            <ModifyForm  setModifyPid={setModifyPid} modifyPid={modifyPid}/>
          </Modal.Body>
          <Modal.Footer>
            <Button variant="secondary" onClick={() => closeModal()}>
                          关闭
            </Button>
            <Button variant="primary" onClick={() => handleModify()}>
                          提交
            </Button>
          </Modal.Footer>
        </Modal>
      </div>
  );
}

const DeleteForm = (props) => {
  const { deletePid, setDeletePid } = props; 
  const handleDeletePidChange = (event) => {
    setDeletePid(event.target.value);
  };

  return (
    <Form id="deleteForm">
        <Form.Group controlId="pid">
          <Form.Label>待删除论文的论文号</Form.Label>
            <Form.Control type="text" placeholder="请输入待删除论文的论文号" value={deletePid} onChange={handleDeletePidChange} />
        </Form.Group>
    </Form>
  );
}

const DeleteButton = (props) => {
    const { loadProjects } = props;
    const [show, setShow] = useState(false);
    const showModal = () => setShow(true);
    const closeModal = () => setShow(false);
    const [deletePid, setDeletePid] = useState("");
    const handle = async (body) => {
      try {
        const res = await fetch(
          `${HOST}:${PORT}/api/project`,{
            method: 'DELETE',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(body),
          },
        );
        const result = await res.json();
        if (res.ok) {
          alert(result.message);
          closeModal();
          loadProjects();
        } else {
          alert(result.message);
        }
      } catch (err) {
        alert(err.message);
      }
    };
    const handleDelete = async () => {
      if (confirm('确认删除该项目吗？')) {
        const { tid } = JSON.parse(localStorage.getItem(DOMAIN));
        const body = { deletePid, tid};
        handle(body);
      }
    };
    return (
      <div>
        <Button variant="outline-warning" size="sm" onClick={() => showModal()}>
                  删除
        </Button>
        <Modal show={show} onHide={() => closeModal()}>
          <Modal.Header closeButton>
            <Modal.Title>删除项目</Modal.Title>
          </Modal.Header>
          <Modal.Body>
            <DeleteForm setDeletePid={setDeletePid} deletePid={deletePid}/>
          </Modal.Body>
          <Modal.Footer>
            <Button variant="secondary" onClick={() => closeModal()}>
                          关闭
            </Button>
            <Button variant="primary" onClick={() => handleDelete()}>
                          删除
            </Button>
          </Modal.Footer>
        </Modal>
      </div>
    );
  }


const MyRow = (props) => {
    const { project } = props;
    const projectTypeMap = {
      "": "无",
      "1": "国家级项目",
      "2": "省部级项目",
      "3": "市厅级项目",
      "4": "企业合作项目",
      "5": "其他类型项目"
    }

    return (
      <tr>
        <td>{project.pid}</td>
        <td>{project.tid}</td>
        <td>{project.pname}</td>
        <td>{project.projectSource}</td>
        <td>{projectTypeMap[project.projectType]}</td>
        <td>{project.totalBudget}</td>
        <td>{project.startYear}</td>
        <td>{project.endYear}</td>
        <td>{project.projectRank}</td>
        <td>{project.undertakeFunding}</td>
      </tr>
    );
  };

const ProjectList = (props) => {
    const {loadProjects, projects} = props;
    useEffect(() => {loadProjects()}, []);
    const rows = projects.map((project, idx) => (
      <MyRow key={idx} project={project} />
    ));
    return (
      <Table bordered hover>
        <thead>
          <tr>
            <th>项目号</th>
            <th>教师号</th>
            <th>项目名称</th>
            <th>项目来源</th>
            <th>项目类型</th>
            <th>总经费</th>
            <th>开始年份</th>
            <th>结束年份</th>
            <th>排名</th>
            <th>承担经费</th>
          </tr>
        </thead>
        <tbody>
          {rows}
        </tbody>
      </Table>
    );
};

const ProjectListTable = (props) => {
    const {projects, loadProjects} = props;
    const { auth } = useContext(teacherContext);

    return (
      <Container>
        {auth && (
          <Col md={12} className="d-flex justify-content-between align-items-center mb-3">
            <PostButton loadProjects={loadProjects} />
            <SearchButton loadProjects={loadProjects} />
            <ModifyButton loadProjects={loadProjects} />
            <DeleteButton loadProjects={loadProjects} />
          </Col>
        )}
        <ProjectList loadProjects={loadProjects} projects={projects} />
      </Container>
    );
}

const SearchButton = (props) => {
  const {loadProjects} = props;
  const [show, setShow] = useState(false);
  const showModal = () => setShow(true);
  const closeModal = () => setShow(false);

  const handleSearch = async () => {
    const form = document.forms.searchForm;
    const tid = form.tid.value;
    const pid = form.pid.value;
    const pname = form.pname.value;
    const projectSource = form.projectSource.value;
    const projectType = form.projectType.value;
    const totalBudget = form.totalBudget.value;
    const startYear = form.startYear.value;
    const endYear = form.endYear.value;
    const projectRank = form.projectRank.value;
    const undertakeFunding = form.undertakeFunding.value;
    const body = { 
      pid, pname, projectSource, projectType, totalBudget, startYear, endYear, projectRank, undertakeFunding, tid 
    };
    await loadProjects(body);
    closeModal();
}

const handleClear = async () => {
  const form = document.forms.searchForm;
  form.reset();
}

  return (
    <div>
        <Button variant="outline-success" size="sm" onClick={() => showModal()}>
            搜索筛选
        </Button>
        <Modal show={show} onHide={() => closeModal()}>
            <Modal.Header closeButton>
                <Modal.Title>搜索筛选</Modal.Title>
            </Modal.Header>
            <Modal.Body>
                <SearchForm/>
            </Modal.Body>
            <Modal.Footer>
                <Button variant="secondary" onClick={() => handleClear() }>
                    清除
                </Button>
                <Button variant="secondary" onClick={() => closeModal() }>
                    关闭
                </Button>
                <Button variant="primary" onClick={() => handleSearch()}>
                    搜索
                </Button>
            </Modal.Footer>
        </Modal>
    </div>
);

}

const SearchForm = (props) => {
  return (
    <Form id="searchForm">
      <Form.Group controlId="tid">
          <Form.Label>教师号</Form.Label>
          <Form.Control type="text" placeholder="请输入教师号"/>
      </Form.Group>
      <Form.Group controlId="pid">
          <Form.Label>项目号</Form.Label>
          <Form.Control type="text" placeholder="请输入项目号"/>
      </Form.Group>
      <Form.Group controlId="pname">
          <Form.Label>项目名称</Form.Label>
          <Form.Control type="text" placeholder="请输入项目名称"/>
      </Form.Group>
      <Form.Group controlId="projectSource">
          <Form.Label>项目来源</Form.Label>
          <Form.Control type="text" placeholder="请输入发表源"/>
      </Form.Group>
      <Form.Group controlId="projectType">
          <Form.Label>项目类型</Form.Label>
          <Form.Control as="select">
            <option value="">无</option>
            <option value="1">国家级项目</option>
            <option value="2">省部级项目</option>
            <option value="3">市厅级项目</option>
            <option value="4">企业合作项目</option>
            <option value="5">其他类型项目</option>
          </Form.Control>
      </Form.Group>
      <Form.Group controlId="totalBudget">
          <Form.Label>总经费</Form.Label>
          <Form.Control type="text" placeholder="请输入项目总经费"/>
      </Form.Group>
      <Form.Group controlId="startYear">
          <Form.Label>开始年份</Form.Label>
          <Form.Control type="text" placeholder="请输入开始年份"/>
      </Form.Group>
      <Form.Group controlId="endYear">
          <Form.Label>开始年份</Form.Label>
          <Form.Control type="text" placeholder="请输入结束年份"/>
      </Form.Group>
      <Form.Group controlId="projectRank">
          <Form.Label>排名</Form.Label>
          <Form.Control type="text" placeholder="请输入排名"/>
      </Form.Group>
      <Form.Group controlId="undertakeFunding">
          <Form.Label>承担经费</Form.Label>
          <Form.Control type="text" placeholder="请输入承担经费"/>
      </Form.Group>
    </Form>
         
  );
}



const ProjectListPage = () => {
  const [projects, setProjects] = useState([]);
  const loadProjects= async (body) => {
    try {      
      const res = await fetch(`${HOST}:${PORT}/api/project/search`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(body)
      });
      const result = await res.json();
      if (res.ok) {
        setProjects(result.data);
      } else {
        alert(result.message);
      }
    } catch (err) {
      alert(err.message);
    }
  };

  const styles = {
    form: {left: "50px",},
  }

  return (
    <div style={styles.form}>
        <ProjectListTable projects = {projects} loadProjects = {loadProjects}/>
    </div>
  );
};
export default ProjectListPage;